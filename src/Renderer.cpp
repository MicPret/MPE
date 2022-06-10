#include <MPE/Graphics/Renderer.hpp>
#include <MPE/Graphics/GraphicsDefines.hpp>
#include <MPE/Graphics/GraphicsBuffers.hpp>
#include <MPE/Graphics/RenderMapper.hpp>
#include <MPE/Graphics/Texture.hpp>
#include <MPE/Graphics/Shader.hpp>
#include <MPE/Graphics/Vertex.hpp>
#include <MPE/Graphics/DataTransfer/GPUPerFrameData.hpp>
#include <MPE/Graphics/DataTransfer/GPUPerMeshData.hpp>
#include <MPE/Graphics/Camera.hpp>
#include <MPE/Graphics/Mesh.hpp>
#include <MPE/Graphics/Material.hpp>
#include <MPE/Core/Log.hpp>
#include <MPE/Core/Transform.hpp>
#include <MPE/Maths/MathsDefines.hpp>

#include <glad/gl.h>

#include <cassert>
#include <cstdint>
#include <utility>
#include <vector>
#include <MPE/Core/Window.hpp>

namespace
{
	constexpr uint8_t INITIALIZED_BIT = 1;
	uint8_t flags = 0;

	mpe::Shader depth_shader;
	mpe::RenderMapper render_mapper;
	constexpr int FIT_IN_BATCH_FAILURE = 0;
	constexpr int FIT_IN_BATCH_SUCCESS = 1;
	constexpr int FIT_IN_BATCH_NEEDS_NEW = 2;

	mpe::GPUPerFrameData frame;
	struct
	{
		mpe::Camera* activeCamera = nullptr;
		std::vector<mpe::PointLight> pointLights;
	} environment;

	constexpr GLuint SHADOW_MAP_WIDTH = 1024, SHADOW_MAP_HEIGHT = 1024;
	constexpr size_t NUM_DEPTH_MAPS = 8;
	std::array<GLuint, mpe::RenderBatch::TEXTURES_PER_BATCH> depth_maps;
	std::array<GLuint, NUM_DEPTH_MAPS> depth_fbos;

	
}

namespace mpe::Renderer
{
	static bool IsInitialized();
	static void PrepareDraw();
	static void DepthPass();
	static void DrawPass();

	void Initialize()
	{
		if (IsInitialized())
			return;

		glEnable(GL_DEPTH_TEST);
		InitializeBuffers();

		glGenFramebuffers(depth_fbos.size(), depth_fbos.data());
		glGenTextures(depth_maps.size(), depth_maps.data());
		for (size_t i = 0; i < depth_maps.size(); i++)
		{
			glBindTexture(GL_TEXTURE_CUBE_MAP, depth_maps[i]);
			for (unsigned j = 0; j < 6; j++)
				glTexImage2D(
					GL_TEXTURE_CUBE_MAP_POSITIVE_X + j, 0, GL_DEPTH_COMPONENT,
					SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT, 0, GL_DEPTH_COMPONENT,
					GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

			glBindFramebuffer(GL_FRAMEBUFFER, depth_fbos[i]);
			glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth_maps[i], 0);
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		depth_shader = LoadShader("shadows");

		//Initialized!
		flags |= INITIALIZED_BIT;
	}

	void Terminate()
	{
		if (!IsInitialized())
			return;

		FreeShader(depth_shader.id);
		TerminateBuffers();

		glDeleteTextures(depth_maps.size(), depth_maps.data());
		glDeleteFramebuffers(depth_fbos.size(), depth_fbos.data());

		flags &= ~INITIALIZED_BIT;
	}

	Camera* GetActiveCamera()
	{
		return environment.activeCamera;
	}

	void SetActiveCamera(Camera* cam)
	{
		environment.activeCamera = cam;
	}

	void SetPointLights(const std::vector<PointLight>& lights)
	{
		environment.pointLights = lights;
	}

	//============================================

	void Render(
		const Shader& shader,
		const Mesh& mesh,
		const Transform& transform,
		const Material& material)
	{
		assert(IsInitialized());
		render_mapper.AddToBatch(shader, mesh, material, transform);
	}

	void Draw()
	{
		assert(IsInitialized());

		PrepareDraw();
		DepthPass();
		DrawPass();
		render_mapper.Reset();
	}

	bool IsInitialized()
	{
		return flags & INITIALIZED_BIT;
	}

	void PrepareDraw()
	{
		if (environment.activeCamera != nullptr)
			frame.cameraData = mpe::GPUCameraData(*environment.activeCamera);
		else
			MPE_ERROR("No active camera!");
		frame.lightData = mpe::GPULightData(environment.pointLights);

		mpe::Renderer::SendFrameData(frame);
	}

	void DepthPass()
	{
		using namespace mpe;
		using namespace mpe::Renderer;

		constexpr auto NUM_FACES = 6;
		const float near = environment.activeCamera->GetNearPlane();
		const float far = environment.activeCamera->GetFarPlane();
		Mat4f projection = Mat4f::Perspective(
			Maths::PI * 0.5f,
			static_cast<float>(SHADOW_MAP_HEIGHT) / static_cast<float>(SHADOW_MAP_WIDTH),
			near,
			far);
		std::vector<Mat4f> shadow_transforms;
		shadow_transforms.resize(environment.pointLights.size() * NUM_FACES);

		for (size_t i = 0; i < environment.pointLights.size(); i++)
		{
			const auto& l = environment.pointLights[i];
			Mat4f view;

			view = Mat4f::LookAt(l.position, l.position + Vec3f::UnitX(), -Vec3f::UnitY());
			shadow_transforms[i * NUM_FACES] = projection * view;
			view = Mat4f::LookAt(l.position, l.position - Vec3f::UnitX(), -Vec3f::UnitY());
			shadow_transforms[i * NUM_FACES + 1] = projection * view;
			view = Mat4f::LookAt(l.position, l.position + Vec3f::UnitY(), Vec3f::UnitZ());
			shadow_transforms[i * NUM_FACES + 2] = projection * view;
			view = Mat4f::LookAt(l.position, l.position - Vec3f::UnitY(), -Vec3f::UnitZ());
			shadow_transforms[i * NUM_FACES + 3] = projection * view;
			view = Mat4f::LookAt(l.position, l.position + Vec3f::UnitZ(), -Vec3f::UnitY());
			shadow_transforms[i * NUM_FACES + 4] = projection * view;
			view = Mat4f::LookAt(l.position, l.position - Vec3f::UnitZ(), -Vec3f::UnitY());
			shadow_transforms[i * NUM_FACES + 5] = projection * view;
		}

		glViewport(0, 0, SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT);
		glUseProgram(depth_shader.id.value);
		for (size_t i = 0; i < environment.pointLights.size(); i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, depth_fbos[i]);
			glClear(GL_DEPTH_BUFFER_BIT);
			glUniformMatrix4fv(
				glGetUniformLocation(depth_shader.id.value, "shadow_matrices[0]"),
				6,
				GL_FALSE,
				shadow_transforms[i * NUM_FACES].raw.data());
			glUniform3fv(glGetUniformLocation(depth_shader.id.value, "light_pos"), 1, &environment.pointLights[i].position.x);

			for (const auto& g : render_mapper.groups)
			{
				const RenderGroup& group = g.second;
				std::array<GLint, 64> vertices_offsets;
				std::array<void*, 64> indices_offsets;
				vertices_offsets[0] = 0;
				indices_offsets[0] = nullptr;

				for (auto& batch : group.batches)
				{
					if (batch.numMeshes == 0)
						continue;

					for (size_t i = 1; i < batch.numMeshes; i++)
					{
						indices_offsets[i] = (void*)(batch.indicesCounts[i - 1]);
						vertices_offsets[i] = batch.verticesCounts[i - 1];
					}

					SendVertexData(batch.vertices, batch.indices);
					SendMeshData(batch.perMeshData);

					glMultiDrawElementsBaseVertex(
						GL_TRIANGLES,
						batch.indicesCounts.data(),
						MPE_VERTEX_TYPE_ENUM,
						(void**)(indices_offsets.data()),
						batch.numMeshes,
						vertices_offsets.data());
				}
			}
		}
	}

	void DrawPass()
	{
		using namespace mpe;
		using namespace mpe::Renderer;

		Window* w = Window::GetActive();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, w->Width(), w->Height());

		for (const auto& g : render_mapper.groups)
		{
			const Shader& shader = g.first;
			const RenderGroup& group = g.second;
			glUseProgram(shader.id.value);
			std::array<GLint, 64> vertices_offsets;
			std::array<void*, 64> indices_offsets;
			vertices_offsets[0] = 0;
			indices_offsets[0] = nullptr;

			auto dloc = glGetUniformLocation(shader.id.value, "depth_maps[0]");
			auto tloc = glGetUniformLocation(shader.id.value, "textures[0]");

			if (dloc != -1)
			{
				std::array<GLint, NUM_DEPTH_MAPS> duniform_values;
				for (size_t i = 0; i < duniform_values.size(); i++)
					duniform_values[i] = RenderBatch::TEXTURES_PER_BATCH + i;
				glUniform1iv(dloc, NUM_DEPTH_MAPS, duniform_values.data());
				glBindTextures(duniform_values[0], NUM_DEPTH_MAPS, depth_maps.data());
			}

			for (auto& batch : group.batches)
			{
				if (batch.numMeshes == 0)
					continue;

				for (GLuint i = 0; i < batch.numTextures; i++)
				{
					glUniform1i(tloc, i);
					glBindTextureUnit(i, batch.textures[i].value);
				}

				for (size_t i = 1; i < batch.numMeshes; i++)
				{
					indices_offsets[i] = (void*)(batch.indicesCounts[i - 1]);
					vertices_offsets[i] = batch.verticesCounts[i - 1];
				}

				SendVertexData(batch.vertices, batch.indices);
				SendMeshData(batch.perMeshData);

				glMultiDrawElementsBaseVertex(
					GL_TRIANGLES,
					batch.indicesCounts.data(),
					MPE_VERTEX_TYPE_ENUM,
					(void**)(indices_offsets.data()),
					batch.numMeshes,
					vertices_offsets.data());
			}
		}
	}
}
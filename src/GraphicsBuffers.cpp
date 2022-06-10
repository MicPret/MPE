#include <MPE/Graphics/GraphicsBuffers.hpp>
#include <MPE/Graphics/GraphicsDefines.hpp>
#include <MPE/Graphics/RenderBatch.hpp>
#include <MPE/Graphics/DataTransfer/GPULightData.hpp>
#include <MPE/Graphics/Camera.hpp>
#include <MPE/Maths/Mat4f.hpp>

#include <glad/gl.h>

#include <array>
#include <cassert>
#include <MPE/Core/Log.hpp>

namespace //data
{
	//buffers
	GLuint vao;
	std::array<GLuint, 4> buffers;

	constexpr size_t VBO_INDEX = 0;
	constexpr size_t EBO_INDEX = 1;
	constexpr size_t FRAME_UBO_INDEX = 2;
	constexpr size_t DRAW_UBO_INDEX = 3;	
	constexpr size_t FBO_INDEX = 4;
}

namespace mpe::Renderer
{
	void InitializeBuffers()
	{
		//VBO and EBO
		glCreateBuffers(buffers.size(), buffers.data());

		glNamedBufferStorage(buffers[VBO_INDEX], sizeof(Vertex) * RenderBatch::VERTICES_PER_BATCH, nullptr, GL_DYNAMIC_STORAGE_BIT);
		glNamedBufferStorage(buffers[EBO_INDEX], sizeof(VertexIndex) * RenderBatch::INDICES_PER_BATCH, nullptr, GL_DYNAMIC_STORAGE_BIT);

		//VAO
		glCreateVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glVertexArrayVertexBuffer(vao, 0, buffers[VBO_INDEX], 0, sizeof(Vertex));
		glVertexArrayElementBuffer(vao, buffers[EBO_INDEX]);

		glEnableVertexArrayAttrib(vao, 0);	//position
		glEnableVertexArrayAttrib(vao, 1);	//texCoords
		glEnableVertexArrayAttrib(vao, 2);	//normal
		
		glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
		glVertexArrayAttribFormat(vao, 1, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, texCoords));
		glVertexArrayAttribFormat(vao, 2, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, normal));

		glVertexArrayAttribBinding(vao, 0, 0);
		glVertexArrayAttribBinding(vao, 1, 0);
		glVertexArrayAttribBinding(vao, 2, 0);

		//UBOs
		glNamedBufferStorage(
			buffers[FRAME_UBO_INDEX],
			4096,
			nullptr,
			GL_DYNAMIC_STORAGE_BIT);
		glNamedBufferStorage(
			buffers[DRAW_UBO_INDEX],
			sizeof(RenderBatch::perMeshData),
			nullptr,
			GL_DYNAMIC_STORAGE_BIT);

		glBindBufferRange(GL_UNIFORM_BUFFER, MPE_CAMERA_BINDING, buffers[FRAME_UBO_INDEX],
			offsetof(GPUPerFrameData, cameraData), sizeof(GPUCameraData));
		glBindBufferRange(GL_UNIFORM_BUFFER, MPE_LIGHT_BINDING, buffers[FRAME_UBO_INDEX],
			offsetof(GPUPerFrameData, lightData), sizeof(GPULightData));
		glBindBufferRange(GL_UNIFORM_BUFFER, MPE_MESH_BINDING, buffers[DRAW_UBO_INDEX],
			0, sizeof(RenderBatch::perMeshData));
	}

	void TerminateBuffers()
	{
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(buffers.size(), buffers.data());
	}

	void SendMeshData(const std::array<GPUPerMeshData, RenderBatch::MESHES_PER_BATCH>& data)
	{
		glNamedBufferSubData(buffers[DRAW_UBO_INDEX], 0, sizeof(data), data.data());
	}

	void SendFrameData(const GPUPerFrameData& data)
	{
		glNamedBufferSubData(buffers[FRAME_UBO_INDEX], 0, sizeof(GPUPerFrameData), &data);
	}

	void SendVertexData(const std::vector<Vertex>& vertices, const std::vector<VertexIndex>& indices)
	{
		glNamedBufferSubData(buffers[VBO_INDEX], 0, vertices.size() * sizeof(Vertex), vertices.data());
		glNamedBufferSubData(buffers[EBO_INDEX], 0, indices.size() * sizeof(VertexIndex), indices.data());
	}

}


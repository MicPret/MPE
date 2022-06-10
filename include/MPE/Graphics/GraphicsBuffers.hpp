#ifndef MPE_GRAPHICS_BUFFERS_HPP
#define MPE_GRAPHICS_BUFFERS_HPP

#include "RenderBatch.hpp"
#include "PointLight.hpp"

#include "DataTransfer/GPUPerFrameData.hpp"

#include <array>

namespace mpe
{
	namespace Renderer
	{
		//Called by Renderer::Initialize()
		void InitializeBuffers();

		//Called by Renderer::Terminate()
		void TerminateBuffers();

		//Called by Renderer::Draw()
		void SendMeshData(const std::array<GPUPerMeshData, RenderBatch::MESHES_PER_BATCH>& data);

		//Called by Renderer::BeginFrame()
		void SendFrameData(const GPUPerFrameData& data);

		//Called by Renderer::Draw
		void SendVertexData(const std::vector<Vertex>& vertices, const std::vector<VertexIndex>& indices);
	}
}

#endif
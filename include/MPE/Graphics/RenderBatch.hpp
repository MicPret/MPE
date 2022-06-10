#ifndef MPE_RENDER_BATCH_HPP
#define MPE_RENDER_BATCH_HPP

#include "Shader.hpp"
#include "Vertex.hpp"
#include "VertexIndex.hpp"
#include "Texture.hpp"
#include "Material.hpp"
#include "Mesh.hpp"
#include "DataTransfer/GPUPerMeshData.hpp"

#include <MPE/Core/Transform.hpp>
#include <vector>
#include <array>

namespace mpe
{
	struct RenderBatch
	{
		static inline constexpr size_t MESHES_PER_BATCH = 64;
		static inline constexpr size_t VERTICES_PER_BATCH = 1 << 17;
		static inline constexpr size_t INDICES_PER_BATCH = VERTICES_PER_BATCH;
		static inline constexpr size_t INVALID_OFFSET = ~0;
		static inline constexpr size_t TEXTURES_PER_BATCH = 8;

		//Total vertices in this batch
		std::vector<Vertex> vertices;

		//Used to address the index of the first mesh, second mesh, third mesh in vertices...
		std::array<int32_t, MESHES_PER_BATCH> verticesCounts;

		//Total vertex indices in this batch
		std::vector<VertexIndex> indices;

		//Used to address the index of the first mesh, second mesh, third mesh in indices...
		std::array<int32_t, MESHES_PER_BATCH> indicesCounts;

		//Properties for every mesh
		std::array<GPUPerMeshData, MESHES_PER_BATCH> perMeshData;

		//Textures used in this batch
		std::array<TextureId, TEXTURES_PER_BATCH> textures;

		//Number of meshes in this batch
		unsigned numMeshes;

		//Number of textures in this batch
		unsigned numTextures;

		RenderBatch();
		void Reset();
		//Returns false if batch is full and operation failed
		bool Push(
			const Mesh& mesh,
			const Material& material,
			const Transform& transform);
	private:
		bool ContainsTexture(TextureId texture, size_t* index);
	};
}

#endif
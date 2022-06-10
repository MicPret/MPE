#include <MPE/Graphics/RenderBatch.hpp>

#include <utility>

namespace mpe
{
	RenderBatch::RenderBatch()
	{
		constexpr size_t VERTICES_PER_TRIANGLE = 3;	//just to avoid magic numbers
		vertices.reserve(MESHES_PER_BATCH * VERTICES_PER_TRIANGLE);
		indices.reserve(MESHES_PER_BATCH * VERTICES_PER_TRIANGLE);
		Reset();
	}

	void RenderBatch::Reset()
	{
		numMeshes = 0;
		numTextures = 0;
		vertices.clear();
		indices.clear();
		verticesCounts.fill(INVALID_OFFSET);
		indicesCounts.fill(INVALID_OFFSET);
		textures.fill(TextureId(0));
		perMeshData.fill(GPUPerMeshData());
	}

	bool RenderBatch::ContainsTexture(TextureId texture, size_t* index)
	{
		size_t& i = *index;
		for (i = 0; (i < textures.size()) && (textures[i].value != 0); i++)
		{
			if (textures[i].value == texture.value)
				return true;
		}
		return false;
	}

	bool RenderBatch::Push(
		const Mesh& mesh,
		const Material& material,
		const Transform& transform)
	{
		if (numMeshes >= MESHES_PER_BATCH)
			return false;
		if (vertices.size() + mesh.vertices.size() >= VERTICES_PER_BATCH)
			return false;
		if (indices.size() + mesh.indices.size() >= INDICES_PER_BATCH)
			return false;

		GPUPerMeshData per_mesh;
		size_t props_num_textures = 0;
		for (const TextureId& texture : material.textures)
		{
			if (texture.value == 0)
				continue;

			size_t index;

			if (!ContainsTexture(texture, &index))	//must be added
			{
				if (numTextures >= textures.size())	//can't add
					return false;
				textures[numTextures] = texture;		//add
				numTextures++;
			}
			per_mesh.textureIndices[props_num_textures] = index;	//new texture index in the next RenderProperties (UBO)
			props_num_textures++;
		}

		//SUCCESS! Let's populate the batch!
		indicesCounts[numMeshes] = mesh.indices.size();
		indices.insert(indices.end(), mesh.indices.begin(), mesh.indices.end());
		verticesCounts[numMeshes] = mesh.vertices.size();
		vertices.insert(vertices.end(), mesh.vertices.begin(), mesh.vertices.end());
		per_mesh.floats = material.floats;
		per_mesh.vecs = material.vecs;
		per_mesh.modelMatrix = transform.GetMatrix();
		perMeshData[numMeshes] = std::move(per_mesh);
		numMeshes++;

		return true;
	}
}
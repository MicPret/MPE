#ifndef MPE_MESH_HPP
#define MPE_MESH_HPP

#include <vector>
#include <cstdint>

#include "Vertex.hpp"
#include "VertexIndex.hpp"

namespace mpe
{
	struct Mesh
	{
		std::vector<Vertex> vertices;
		std::vector<VertexIndex> indices;
	};

	inline constexpr auto MESHES_PATH_PROPERTY = "MESHES_PATH";
}

#endif
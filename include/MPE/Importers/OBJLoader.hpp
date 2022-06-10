#ifndef MPE_OBJ_LOADER_HPP
#define MPE_OBJ_LOAFER_HPP

#include <MPE/Graphics/Mesh.hpp>
#include <string>

namespace mpe
{
	Mesh LoadOBJMesh(const std::string& filename);
}

#endif
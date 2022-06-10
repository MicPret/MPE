#ifndef MPE_VERTEX_HPP
#define MPE_VERTEX_HPP

#include <MPE/Maths/Vec2f.hpp>
#include <MPE/Maths/Vec3f.hpp>

namespace mpe
{
	struct Vertex
	{
		Vec3f position;
		Vec2f texCoords;
		Vec3f normal;
	};
}

#endif
#ifndef MPE_POINT_LIGHT_HPP
#define MPE_POINT_LIGHT_HPP

#include <MPE/Maths/Vec3f.hpp>

namespace mpe
{
	struct PointLight
	{
		Vec3f position;
		Vec3f ambient;
		Vec3f diffuse;
		Vec3f specular;
		float constant;
		float linear;
		float quadratic;
	};
}

#endif
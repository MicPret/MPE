#ifndef MPE_GPU_POINT_LIGHT_HPP
#define MPE_GPU_POINT_LIGHT_HPP

#include <MPE/Maths/Vec4f.hpp>
#include <MPE/Graphics/PointLight.hpp>

namespace mpe
{
	struct GPUPointLight
	{
		Vec4f positionConstant;
		Vec4f ambientLinear;
		Vec4f diffuseQuadratic;
		Vec4f specular;

		constexpr GPUPointLight() = default;
		GPUPointLight(const PointLight& light);
	};
}

#endif
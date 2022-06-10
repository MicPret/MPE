#include <MPE/Graphics/DataTransfer/GPUPointLight.hpp>

namespace mpe
{
	GPUPointLight::GPUPointLight(const PointLight& light) :
		positionConstant(light.position, light.constant),
		ambientLinear(light.ambient, light.linear),
		diffuseQuadratic(light.diffuse, light.quadratic),
		specular(light.specular, 1.0f)
	{ }
}
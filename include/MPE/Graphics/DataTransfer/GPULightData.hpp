#ifndef MPE_GPU_LIGHT_DATA_HPP
#define MPE_GPU_LIGHT_DATA_HPP

#include "GPUPointLight.hpp"
#include <array>
#include <vector>

namespace mpe
{
	struct GPULightData
	{
		inline static constexpr size_t NUM_POINT_LIGHTS = 8;

		std::array<GPUPointLight, NUM_POINT_LIGHTS> pointLights;
		int numPointLights;
		std::array<unsigned, NUM_POINT_LIGHTS> shadowMapIds;

		inline GPULightData() : numPointLights(0) { pointLights.fill(GPUPointLight()); shadowMapIds.fill(0); }
		GPULightData(const std::vector<PointLight>& point_lights);
	};
}

#endif
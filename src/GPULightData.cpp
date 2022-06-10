#include <MPE/Graphics/DataTransfer/GPULightData.hpp>

#include <algorithm>
#include <assert.h>

namespace mpe
{
	GPULightData::GPULightData(const std::vector<PointLight>& point_lights) :
		numPointLights(point_lights.size())
	{
		assert(numPointLights <= pointLights.size());
		std::transform(point_lights.begin(), point_lights.end(), pointLights.begin(),
			[](const PointLight& p) { return GPUPointLight(p); });
	}
}
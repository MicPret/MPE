#ifndef MPE_GPU_FRAME_DATA
#define MPE_GPU_FRAME_DATA

#include "GPUCameraData.hpp"
#include "GPULightData.hpp"

namespace mpe
{
	struct GPUPerFrameData
	{
		GPUCameraData cameraData;
		std::array<unsigned char, 256 - (sizeof(GPUCameraData) % 256)> padding;	//remove if sizeof(cameraData) = 256
		GPULightData lightData;
	};
}

#endif
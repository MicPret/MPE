#ifndef MPE_GPU_CAMERA_DATA_HPP
#define MPE_GPU_CAMERA_DATA_HPP

#include <MPE/Graphics/Camera.hpp>

namespace mpe
{
	struct GPUCameraData
	{
		Mat4f projection;
		Mat4f view;
		Vec4f position;
		float near;
		float far;

		inline GPUCameraData() :
			projection(Mat4f::Identity()),
			view(Mat4f::Identity()),
			near(0.0f),
			far(0.0f)
		{ }
		GPUCameraData(const Camera& camera);
	};
}

#endif
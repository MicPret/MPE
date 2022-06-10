#include <MPE/Graphics/DataTransfer/GPUCameraData.hpp>

namespace mpe
{
	GPUCameraData::GPUCameraData(const Camera& camera) :
		projection(camera.GetProjectionMatrix()),
		view(camera.GetViewMatrix()),
		position(camera.GetTransform().position, 1.0f),
		near(camera.GetNearPlane()),
		far(camera.GetFarPlane())
	{ }
}
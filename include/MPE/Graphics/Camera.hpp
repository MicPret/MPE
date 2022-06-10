#ifndef MPE_CAMERA_HPP
#define MPE_CAMERA_HPP

#include <MPE/Maths/Mat4f.hpp>
#include <MPE/Core/Transform.hpp>

namespace mpe
{
	class Camera
	{
	public:
		Camera(float fovy, float aspect, float near, float far);
		Mat4f GetViewMatrix() const;
		Mat4f GetProjectionMatrix() const;
		const Transform& GetTransform() const;
		void SetTransform(const Transform& transf);
		void SetFov(float fovy);
		float GetFov() const;
		float GetNearPlane() const;
		float GetFarPlane() const;
	private:
		Transform transform;
		float fov;
		float ratio;
		float n;
		float f;
	};
}

#endif
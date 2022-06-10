#include <MPE/Graphics/Camera.hpp>

#include <cassert>

namespace mpe
{
	Camera::Camera(float fovy, float aspect, float near, float far) :
		fov(fovy),
		ratio(aspect),
		n(near),
		f(far)
	{ }

	Mat4f Camera::GetViewMatrix() const
	{
		Vec3f forward = transform.rotation.Rotate(Vec3f(0.0f, 0.0f, -1.0f)).Normalized();
		Vec3f right = forward.Cross(Vec3f(0.0f, 1.0f, 0.0f)).Normalized();
		Vec3f up = right.Cross(forward).Normalized();

		return Mat4f::LookAt(transform.position, right, up, forward);
	}

	Mat4f Camera::GetProjectionMatrix() const
	{
		return Mat4f::Perspective(fov, ratio, n, f);
	}

	const Transform& Camera::GetTransform() const
	{
		return transform;
	}

	void Camera::SetTransform(const Transform& transf)
	{
		transform = transf;
	}

	void Camera::SetFov(float fovy)
	{
		fov = fovy;
	}

	float Camera::GetFov() const
	{
		return fov;
	}

	float Camera::GetNearPlane() const
	{
		return n;
	}
	
	float Camera::GetFarPlane() const
	{
		return f;
	}
}
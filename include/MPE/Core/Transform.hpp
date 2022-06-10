#ifndef MPE_TRANSFORM_HPP
#define MPE_TRANSFORM_HPP

#include <MPE/Maths/Vec3f.hpp>
#include <MPE/Maths/Quaternion.hpp>
#include <MPE/Maths/Mat4f.hpp>

namespace mpe
{
	struct Transform
	{
		Vec3f position;
		Quaternion rotation;
		Vec3f scale;

		constexpr Transform() : scale(1.0f, 1.0f, 1.0f)
		{ }

		inline Vec3f Forward()
		{
			return rotation.Rotate(Vec3f::UnitZ()).Normalized();
		}

		inline Vec3f Right()
		{
			return Vec3f::UnitY().Cross(Forward()).Normalized();
		}

		inline Vec3f Up()
		{
			return Forward().Cross(Right()).Normalized();
		}

		constexpr Mat4f GetMatrix() const
		{
			Mat4f matrix;

			matrix[0] = scale.x;
			matrix[5] = scale.y;
			matrix[10] = scale.z;
			matrix[12] = position.x;
			matrix[13] = position.y;
			matrix[14] = position.z;
			matrix[15] = 1.0f;

			Mat4f rot;
			const Quaternion& r = rotation;
			rot[0] = 1.0f - 2.0f * (r.y * r.y + r.z * r.z);
			rot[1] = 2.0f * (r.x * r.y + r.w * r.z);
			rot[2] = 2.0f * (r.x * r.z - r.w * r.y);
			rot[4] = 2.0f * (r.x * r.y - r.w * r.z);
			rot[5] = 1.0f - 2.0f * (r.x * r.x + r.z * r.z);
			rot[6] = 2.0f * (r.y * r.z + r.w * r.x);
			rot[8] = 2.0f * (r.x * r.z + r.w * r.y);
			rot[9] = 2.0f * (r.y * r.z - r.w * r.x);
			rot[10] = 1.0f - 2.0f * (r.x * r.x + r.y * r.y);
			rot[15] = 1.0f;

			return matrix * rot;	//TODO optimize!!!
		}
	};
}

#endif
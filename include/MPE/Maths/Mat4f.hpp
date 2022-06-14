#ifndef MPE_MAT4F_HPP
#define MPE_MAT4F_HPP

#include "Vec3f.hpp"
#include "Vec4f.hpp"

#include <cstddef>
#include <array>

namespace mpe
{
	struct Mat4f
	{
		//[ U0(3), 0, U1(3), 0, U2(3), 0, T(3), 1 ]
		std::array<float, 16> raw;
		
		constexpr Mat4f() : raw {
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f }
		{}
		constexpr Mat4f(float d) : raw {
			d, 0.0f, 0.0f, 0.0f,
			0.0f, d, 0.0f, 0.0f,
			0.0f, 0.0f, d, 0.0f,
			0.0f, 0.0f, 0.0f, d }
		{}


		constexpr float operator[](size_t index) const { return raw[index]; }
		constexpr float& operator[](size_t index) { return raw[index]; }

		//Returns an identity 4x4 matrix
		static constexpr Mat4f Identity() { return Mat4f(1.0f); }
		//Returns a perspective projection 4x4 matrix
		static Mat4f Perspective(float fovy, float aspect, float near, float far);

		static constexpr Mat4f Translation(float x, float y, float z)
		{
			Mat4f result = Identity();
			result[12] = x;
			result[13] = y;
			result[14] = z;
			return result;
		}

		static constexpr Mat4f Translation(Vec3f translation)
		{
			return Translation(translation.x, translation.y, translation.z);
		}

		static Mat4f Rotation(const Vec3f& unit_axis, float radians);

		static constexpr Mat4f Scale(float x, float y, float z)
		{
			Mat4f result;
			result[0] = x;
			result[5] = y;
			result[10] = z;
			return result;
		}

		static constexpr Mat4f Scale(const Vec3f& scale)
		{
			return Scale(scale.x, scale.y, scale.z);
		}

		static constexpr Mat4f LookAt(const Vec3f& eye, const Vec3f& right, const Vec3f& up, const Vec3f& forward)
		{
			Vec3f e = -eye;
			Mat4f mat = Mat4f::Identity();
			mat[0] = right.x;
			mat[1] = up.x;
			mat[2] = forward.x;
			mat[4] = right.y;
			mat[5] = up.y;
			mat[6] = forward.y;
			mat[8] = right.z;
			mat[9] = up.z;
			mat[10] = forward.z;
			mat[12] = right.Dot(e);
			mat[13] = up.Dot(e);
			mat[14] = forward.Dot(e);
			return mat;
		}

		static Mat4f LookAt(const Vec3f& eye, const Vec3f& target, const Vec3f& up)
		{
			Vec3f forward = (target - eye).Normalized();
			Vec3f u = up.Normalized();
			Vec3f right = forward.Cross(u).Normalized();

			Mat4f mat = Mat4f::Identity();
			mat[0] = right.x;
			mat[1] = u.x;
			mat[2] = -forward.x;
			mat[4] = right.y;
			mat[5] = u.y;
			mat[6] = -forward.y;
			mat[8] = right.z;
			mat[9] = u.z;
			mat[10] = -forward.z;
			mat[12] = -right.Dot(eye);
			mat[13] = -u.Dot(eye);
			mat[14] = forward.Dot(eye);
			return mat;
		}
	};

	constexpr Mat4f operator*(const Mat4f& a, const Mat4f& b)
	{
		Mat4f result;
		result[0] = a[0] * b[0] + a[4] * b[1] + a[8] * b[2] + a[12] * b[3];
		result[1] = a[1] * b[0] + a[5] * b[1] + a[9] * b[2] + a[13] * b[3];
		result[2] = a[2] * b[0] + a[6] * b[1] + a[10] * b[2] + a[14] * b[3];
		result[3] = a[3] * b[0] + a[7] * b[1] + a[11] * b[2] + a[15] * b[3];
		result[4] = a[0] * b[4] + a[4] * b[5] + a[8] * b[6] + a[12] * b[7];
		result[5] = a[1] * b[4] + a[5] * b[5] + a[9] * b[6] + a[13] * b[7];
		result[6] = a[2] * b[4] + a[6] * b[5] + a[10] * b[6] + a[14] * b[7];
		result[7] = a[3] * b[4] + a[7] * b[5] + a[11] * b[6] + a[15] * b[7];
		result[8] = a[0] * b[8] + a[4] * b[9] + a[8] * b[10] + a[12] * b[11];
		result[9] = a[1] * b[8] + a[5] * b[9] + a[9] * b[10] + a[13] * b[11];
		result[10] = a[2] * b[8] + a[6] * b[9] + a[10] * b[10] + a[14] * b[11];
		result[11] = a[3] * b[8] + a[7] * b[9] + a[11] * b[10] + a[15] * b[11];
		result[12] = a[0] * b[12] + a[4] * b[13] + a[8] * b[14] + a[12] * b[15];
		result[13] = a[1] * b[12] + a[5] * b[13] + a[9] * b[14] + a[13] * b[15];
		result[14] = a[2] * b[12] + a[6] * b[13] + a[10] * b[14] + a[14] * b[15];
		result[15] = a[3] * b[12] + a[7] * b[13] + a[11] * b[14] + a[15] * b[15];
		return result;
	}

	constexpr Vec4f operator*(const Mat4f& mat, const Vec4f& vec)
	{
		Vec4f result;
		result.x = mat[0] * vec.x + mat[4] * vec.y + mat[8] * vec.z + mat[12] * vec.w;
		result.y = mat[1] * vec.x + mat[5] * vec.y + mat[9] * vec.z + mat[13] * vec.w;
		result.z = mat[2] * vec.x + mat[6] * vec.y + mat[10] * vec.z + mat[14] * vec.w;
		result.w = mat[3] * vec.x + mat[7] * vec.y + mat[11] * vec.z + mat[15] * vec.w;
		return result;
	}
}

#endif
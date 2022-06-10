#include <MPE/Maths/Mat4f.hpp>

#include <cmath>
#include <cassert>

namespace mpe
{
	//stolen from glm (thanks/sorry)
	Mat4f Mat4f::Perspective(float fovy, float aspect, float near, float far)
	{
		const float tanHalfFovy = std::tan(fovy * 0.5f);
		const float inverse_depth = 1.0f / (far - near);

		Mat4f result;
		result[0] = 1.0f / (aspect * tanHalfFovy);
		result[5] = 1.0f / tanHalfFovy;
		result[10] = -(far + near) * inverse_depth;
		result[11] = -1.0f;
		result[14] = -(2.0f * far * near) * inverse_depth;
		return result;
	}

	Mat4f Mat4f::Rotation(const Vec3f& unit_axis, float radians)
	{
		assert(unit_axis.IsUnitVector());
		Mat4f result = Mat4f::Identity();
		float cosine = std::cos(radians);
		float comp_cos = 1.0f - cosine;
		float sine = std::sin(radians);

		result[0] = cosine + unit_axis.x * unit_axis.x * comp_cos;
		result[1] = unit_axis.x * unit_axis.y * comp_cos + unit_axis.z * sine;
		result[2] = unit_axis.x * unit_axis.z * comp_cos - unit_axis.y * sine;
		//result[3] = 0.0f;
		result[4] = unit_axis.x * unit_axis.y * comp_cos - unit_axis.z * sine;
		result[5] = cosine + unit_axis.y * unit_axis.y * comp_cos;
		result[6] = unit_axis.y * unit_axis.z * comp_cos + unit_axis.x * sine;
		//result[7] = 0.0f;
		result[8] = unit_axis.x * unit_axis.z * comp_cos + unit_axis.y * sine;
		result[9] = unit_axis.y * unit_axis.z * comp_cos - unit_axis.x * sine;
		result[10] = cosine + unit_axis.z * unit_axis.z * comp_cos;
		//result[11] = 0.0f;
		//result[15] = 1.0f;

		return result;
	}

}
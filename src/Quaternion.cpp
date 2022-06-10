#include <MPE/Maths/Quaternion.hpp>

#include <cmath>
#include <cassert>

namespace mpe
{
	Quaternion::Quaternion(const Vec3f& unit_axis, float radians)
	{
		assert(unit_axis.IsUnitVector());
		float sine = std::sin(radians * 0.5f);
		float cosine = std::cos(radians * 0.5f);

		x = unit_axis.x * sine;
		y = unit_axis.y * sine;
		z = unit_axis.z * sine;
		w = cosine;
	}
}
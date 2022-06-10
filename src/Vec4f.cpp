#include <MPE/Maths/Vec4f.hpp>
#include <MPE/Maths/MathsDefines.hpp>

#include <cmath>

namespace mpe
{
	float Vec4f::Magnitude() const
	{
		return std::sqrt(SquaredMagnitude());
	}

	void Vec4f::Normalize()
	{
		float inverse_magnitude = 1.0f / Magnitude();
		x *= inverse_magnitude;
		y *= inverse_magnitude;
		z *= inverse_magnitude;
		w *= inverse_magnitude;
	}
}
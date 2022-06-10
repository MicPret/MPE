#include <MPE/Maths/Vec2f.hpp>
#include <MPE/Maths/MathsDefines.hpp>

#include <cmath>

namespace mpe
{
	float Vec2f::Magnitude() const
	{
		return std::sqrt(SquaredMagnitude());
	}

	void Vec2f::Normalize()
	{
		float inverse_magnitude = 1.0f / Magnitude();
		x *= inverse_magnitude;
		y *= inverse_magnitude;
	}
}
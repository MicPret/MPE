#include <MPE/Maths/Vec3f.hpp>
#include <MPE/Maths/MathsDefines.hpp>

#include <cmath>

namespace mpe
{
	float Vec3f::Magnitude() const
	{
		return std::sqrt(SquaredMagnitude());
	}

	Vec3f Vec3f::Normalized() const
	{
		float inverse_magnitude = 1.0f / Magnitude();
		return inverse_magnitude * (*this);
	}
}
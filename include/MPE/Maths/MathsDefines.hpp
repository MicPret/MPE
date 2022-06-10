#ifndef MPE_MATHS_DEFINES_HPP
#define MPE_MATHS_DEFINES_HPP

namespace mpe::Maths
{
	inline constexpr float EPSILON = 1e-6f;
	inline constexpr float PI = 3.14159265359f;

	constexpr bool AreAlmostEqual(float a, float b, float epsilon = EPSILON)
	{
		float c = a - b;
		return (c >= 0.0f && c < epsilon) || (c < 0.0f && -c < epsilon);
	}
}

#endif
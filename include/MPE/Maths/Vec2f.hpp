#ifndef MPE_VEC2F_HPP
#define MPE_VEC2F_HPP

#include "MathsDefines.hpp"

namespace mpe
{
	struct Vec2f
	{
		float x;
		float y;

		constexpr Vec2f(float x_, float y_) : x(x_), y(y_)
		{ }

		constexpr Vec2f() : x(0), y(0)
		{ }

		//Computes the dot product
		constexpr float Dot(const Vec2f& other) const
		{
			return x * other.x + y * other.y;
		}

		//Computes the squared magnitude, faster than Magnitude()
		constexpr float SquaredMagnitude() const
		{
			return Dot(*this);
		}

		//Computes the magnitude, slower than SquaredMagnitude()
		float Magnitude() const;

		//Checks whether it's a unit vector
		constexpr bool IsUnitVector() const
		{
			return Maths::AreAlmostEqual(SquaredMagnitude(), 1.0f, Maths::EPSILON * Maths::EPSILON);
		}

		void Normalize();
	};

	constexpr Vec2f operator +(const Vec2f& a, const Vec2f& b)
	{
		return Vec2f(a.x + b.x, a.y + b.y);
	}
}

#endif
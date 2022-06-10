#ifndef MPE_VEC4F_HPP
#define MPE_VEC4F_HPP

#include "MathsDefines.hpp"
#include "Vec3f.hpp"

namespace mpe
{
	struct Vec4f
	{
		float x;
		float y;
		float z;
		float w;

		constexpr Vec4f(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_)
		{ }

		constexpr Vec4f() : x(0.0f), y(0.0f), z(0.0f), w(0.0f)
		{ }

		constexpr Vec4f(Vec3f vec, float w_) : x(vec.x), y(vec.y), z(vec.z), w(w_)
		{ }

		//Computes the dot product
		constexpr float Dot(const Vec4f& other) const
		{
			return x * other.x + y * other.y + z * other.z + w * other.w;
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
			return Maths::AreAlmostEqual(SquaredMagnitude(), Maths::EPSILON * Maths::EPSILON);
		}

		//Extracts the x, y and z components
		constexpr Vec3f Xyz() const
		{
			return Vec3f(x, y, z);
		}

		void Normalize();
	};

	constexpr Vec4f operator+(const Vec4f& a, const Vec4f& b)
	{
		return Vec4f(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
	}
}

#endif
#ifndef MPE_VEC3F_HPP
#define MPE_VEC3F_HPP

#include "MathsDefines.hpp"

namespace mpe
{
	struct Vec3f
	{
		float x;
		float y;
		float z;

		constexpr Vec3f(float x_, float y_, float z_) : x(x_), y(y_), z(z_)
		{ }

		constexpr Vec3f() : x(0.0f), y(0.0f), z(0.0f)
		{ }

		//Computes the dot product
		constexpr float Dot(const Vec3f& other) const
		{
			return x * other.x + y * other.y + z * other.z;
		}

		//Computes the cross product
		constexpr Vec3f Cross(const Vec3f& other) const
		{
			return Vec3f(
				y * other.z - other.y * z,
				z * other.x - other.z * x,
				x * other.y - other.x * y
			);
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
			return Maths::AreAlmostEqual(SquaredMagnitude(), 1.0f, Maths::EPSILON);
		}

		Vec3f Normalized() const;

		static constexpr Vec3f UnitX()
		{
			return Vec3f(1.0f, 0.0f, 0.0f);
		}

		static constexpr Vec3f UnitY()
		{
			return Vec3f(0.0f, 1.0f, 0.0f);
		}

		static constexpr Vec3f UnitZ()
		{
			return Vec3f(0.0f, 0.0f, 1.0f);
		}
	};

	constexpr Vec3f operator+(const Vec3f& a, const Vec3f& b)
	{
		return Vec3f(a.x + b.x, a.y + b.y, a.z + b.z);
	}

	constexpr Vec3f operator-(const Vec3f& v)
	{
		return Vec3f(-v.x, -v.y, -v.z);
	}

	constexpr Vec3f operator-(const Vec3f& a, const Vec3f& b)
	{
		return Vec3f(a.x - b.x, a.y - b.y, a.z - b.z);
	}

	constexpr Vec3f operator*(float scalar, const Vec3f& vec)
	{
		return Vec3f(scalar * vec.x, scalar * vec.y, scalar * vec.z);
	}
}

#endif
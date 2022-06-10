#ifndef MPE_QUATERNION_HPP
#define MPE_QUATERNION_HPP

#include "MathsDefines.hpp"
#include "Vec3f.hpp"

namespace mpe
{
	//Unit-length quaternions are used to represent 3D rotations
	struct Quaternion
	{
		float x;
		float y;
		float z;
		float w;

		constexpr Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) { }
		constexpr Quaternion(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) { }
		Quaternion(const Vec3f& unit_axis, float radians);

		constexpr float SquaredMagnitude() const { return x * x + y * y + z * z + w * w; }
		//In case of unit-length quaternions, Conjugate() computes the inverse quaternion
		constexpr Quaternion Conjugate() const { return Quaternion(-x, -y, -z, w); }
		//Use Conjugate() when dealing with unit-length quaternions instead
		constexpr Quaternion Inverse() const
		{
			float ism = 1.0f / SquaredMagnitude();	//inverse squared magnitude
			Quaternion conj = Conjugate();
			return Quaternion(conj.x * ism, conj.y * ism, conj.z * ism, conj.w * ism);
		}
		//This is supposed to always return true
		constexpr bool IsUnitLength() const { return Maths::AreAlmostEqual(SquaredMagnitude(), 1.0f, Maths::EPSILON * Maths::EPSILON); }

		//Returns a rotated vector. WARNING: Rotate() assumes this is a unit-length quaternion
		constexpr Vec3f Rotate(const Vec3f& vec) const;
	};

	//Computes the result of rotation p applied after rotation q
	constexpr Quaternion operator*(const Quaternion& p, const Quaternion& q)
	{
		Vec3f vp(p.x, p.y, p.z);
		Vec3f vq(q.x, q.y, q.z);
		Vec3f vec = p.w * vq + q.w * vp + vp.Cross(vq);
		return Quaternion(vec.x, vec.y, vec.z, p.w * q.w - vp.Dot(vq));
	}

	constexpr Vec3f Quaternion::Rotate(const Vec3f& vec) const
	{
		Quaternion v(vec.x, vec.y, vec.z, 0.0f);
		v = *this * v * Conjugate();
		return Vec3f(v.x, v.y, v.z);
	}
}

#endif
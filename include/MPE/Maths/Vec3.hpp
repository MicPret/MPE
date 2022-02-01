#ifndef MPE_VEC3_HPP
#define MPE_VEC3_HPP

namespace mpe
{
	template <typename T>
	struct Vec3
	{
		T x;
		T y;
		T z;

		constexpr Vec3(T x_, T y_, T z_) : x(x_), y(y_), z(z_)
		{ }

		constexpr Vec3() : x(0), y(0), z(0)
		{ }
	};

	using Vec3f = Vec3<float>;
}

#endif
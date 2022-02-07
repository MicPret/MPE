#ifndef MPE_SYSTEM_BUCKET_HPP
#define MPE_SYSTEM_BUCKET_HPP

#include "Entity.hpp"

namespace mpe
{
	enum class SystemBucket : unsigned char
	{
		PreRender = 0,
		Default,
		Render,
		PostRender,
		Count
	};

	constexpr bool operator < (SystemBucket a, SystemBucket b)
	{
		return static_cast<unsigned char>(a) < static_cast<unsigned char>(b);
	}
}

#endif
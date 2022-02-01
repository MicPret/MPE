#ifndef MPE_UTILS_HPP
#define MPE_UTILS_HPP

#include <utility>

namespace mpe
{
	//Lets the user access one global variable for every default-constructible type.
	//For instance, MPE uses a global Logger. Users can access the same Logger
	//and/or instantiate their own
	template <typename T>
	T& Global()
	{
		static T globl;
		return globl;
	}
}

#endif
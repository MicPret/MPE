#ifndef MPE_IDENTIFIER_HPP
#define MPE_IDENTIFIER_HPP

#include <cstdint>

namespace mpe
{
	template <typename T>
	struct Identifier
	{
		uint32_t value;
		constexpr Identifier() : value(0u) { }
		constexpr Identifier(uint32_t value_) : value(value_) { }
		constexpr bool operator==(Identifier<T> other) const { return value == other.value; }
		constexpr bool operator!=(Identifier<T> other) const { return value != other.value; }
		constexpr bool IsValid() const { return value != 0; }
	};
}

#endif
#ifndef MPE_UTILS_HPP
#define MPE_UTILS_HPP

#include <utility>
#include <string_view>
#include <cstdint>

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

    //Returns the name of the specified type at compile time
    template <typename T>
    constexpr std::string_view TypeName();

    template <>
    constexpr std::string_view TypeName<void>() { return "void"; }

    constexpr uint32_t Hash(std::string_view s)
    {
        size_t size = s.size();
        return size > 0 ?
            (Hash(s.substr(0, size - 1)) ^ s[size - 1]) * 16777619u :
            2166136261u;
    }

    namespace detail
    {
        using type_name_prober = void;

        template <typename T>
        constexpr std::string_view InternalTypeName()
        {
#ifdef __clang__
            return __PRETTY_FUNCTION__;
#elif defined(__GNUC__)
            return __PRETTY_FUNCTION__;
#elif defined(_MSC_VER)
            return __FUNCSIG__;
#else
#error "Unsupported compiler"
#endif
        }

        constexpr std::size_t InternalTypeNamePrefixLength() {
            return InternalTypeName<type_name_prober>().find(TypeName<type_name_prober>());
        }

        constexpr std::size_t InternalTypeNameSuffixLength() {
            return InternalTypeName<type_name_prober>().length()
                - InternalTypeNamePrefixLength()
                - TypeName<type_name_prober>().length();
        }

    }

    template <typename T>
    constexpr std::string_view TypeName()
    {
        constexpr std::string_view internal_name = detail::InternalTypeName<T>();
        constexpr size_t prefix_length = detail::InternalTypeNamePrefixLength();
        constexpr size_t suffix_length = detail::InternalTypeNameSuffixLength();
        constexpr size_t type_name_length = internal_name.length() - (prefix_length + suffix_length);
        return internal_name.substr(prefix_length, type_name_length);
    }
}

#endif
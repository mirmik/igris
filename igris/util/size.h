#ifndef IGRIS_UTIL_SIZE_H
#define IGRIS_UTIL_SIZE_H

#include <utility>

namespace igris
{
	template <class C>
	constexpr auto size(const C& c) -> decltype(c.size())
	{
		return c.size();
	}

	template <class C>
	constexpr auto ssize(const C& c)
	-> std::common_type_t<std::ptrdiff_t,
	std::make_signed_t<decltype(c.size())>>
	{
		using R = std::common_type_t<std::ptrdiff_t,
		std::make_signed_t<decltype(c.size())>>;
		return static_cast<R>(c.size());
	}

	template <class T, std::size_t N>
	constexpr std::size_t size(const T (&array)[N]) noexcept
	{
		return N;
	}

	template <class T, std::ptrdiff_t N>
	constexpr std::ptrdiff_t ssize(const T (&array)[N]) noexcept
	{
		return N;
	}
}

#endif
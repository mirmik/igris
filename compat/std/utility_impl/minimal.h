#ifndef IGRIS_STD_UTILITY_MINIMAL_H
#define IGRIS_STD_UTILITY_MINIMAL_H
#include "igris_std_namespace.h"

#include <type_traits_impl/standalone.h>

namespace igris_std
{
    // igris_std::declval
    template <class T>
    typename igris_std::add_rvalue_reference<T>::type declval() noexcept;

    // igris_std::forward
    template <class T>
    constexpr T &&forward(typename igris_std::remove_reference<T>::type &t) noexcept;

    template <class T>
    constexpr T &&forward(typename igris_std::remove_reference<T>::type &&t) noexcept;
}

#endif

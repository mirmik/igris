#ifndef IGRIS_STD_TYPE_TRAITS_RESULT_H
#define IGRIS_STD_TYPE_TRAITS_RESULT_H
#include "igris_std_namespace.h"

#include <type_traits_impl/decay.h>
#include <type_traits_impl/standalone.h>

namespace igris_std
{
    // REFERENCE_WRAPER PREDECLARE
    template <class T> struct reference_wrapper;

    template <class T> struct is_reference_wrapper : igris_std::false_type
    {
    };
    template <class U>
    struct is_reference_wrapper<igris_std::reference_wrapper<U>> : igris_std::true_type
    {
    };

    namespace detail
    {
        template <class T> struct invoke_impl
        {
            template <class F, class... Args>
            static auto call(F &&f, Args &&... args)
                -> decltype(igris_std::forward<F>(f)(igris_std::forward<Args>(args)...));
        };

        template <class B, class MT> struct invoke_impl<MT B::*>
        {
            template <class T,
                      class Td = typename igris_std::decay<T>::type,
                      class = typename igris_std::enable_if<
                          igris_std::is_base_of<B, Td>::value>::type>
            static auto get(T &&t) -> T &&;

            template <class T,
                      class Td = typename igris_std::decay<T>::type,
                      class = typename igris_std::enable_if<
                          is_reference_wrapper<Td>::value>::type>
            static auto get(T &&t) -> decltype(t.get());

            template <class T,
                      class Td = typename igris_std::decay<T>::type,
                      class = typename igris_std::enable_if<
                          !igris_std::is_base_of<B, Td>::value>::type,
                      class = typename igris_std::enable_if<
                          !is_reference_wrapper<Td>::value>::type>
            static auto get(T &&t) -> decltype(*igris_std::forward<T>(t));

            template <class T,
                      class... Args,
                      class MT1,
                      class = typename igris_std::enable_if<
                          igris_std::is_function<MT1>::value>::type>
            static auto call(MT1 B::*pmf, T &&t, Args &&... args)
                -> decltype((invoke_impl::get(igris_std::forward<T>(t)).*
                             pmf)(igris_std::forward<Args>(args)...));

            template <class T>
            static auto call(MT B::*pmd, T &&t)
                -> decltype(invoke_impl::get(igris_std::forward<T>(t)).*pmd);
        };

        template <class F,
                  class... Args,
                  class Fd = typename igris_std::decay<F>::type>
        auto INVOKE(F &&f, Args &&... args)
            -> decltype(invoke_impl<Fd>::call(igris_std::forward<F>(f),
                                              igris_std::forward<Args>(args)...));

    } // namespace detail

    // Minimal C++11 implementation:
    /*	template <class> struct result_of;
        template <class F, class... ArgTypes>
        struct result_of<F(ArgTypes...)>
        {
            using type = decltype(detail::INVOKE(igris_std::declval<F>(),
       igris_std::declval<ArgTypes>()...));
        };*/

    // Conforming C++14 implementation (is also a valid C++11 implementation):
    namespace detail
    {
        template <typename AlwaysVoid, typename, typename...>
        struct invoke_result
        {
        };

        template <typename F, typename... Args>
        struct invoke_result<decltype(void(detail::INVOKE(
                                 igris_std::declval<F>(), igris_std::declval<Args>()...))),
                             F,
                             Args...>
        {
            using type = decltype(
                detail::INVOKE(igris_std::declval<F>(), igris_std::declval<Args>()...));
        };
    } // namespace detail

    template <class> struct result_of;

    template <class F, class... ArgTypes>
    struct result_of<F(ArgTypes...)>
        : detail::invoke_result<void, F, ArgTypes...>
    {
    };

    template <class F, class... ArgTypes>
    struct invoke_result : detail::invoke_result<void, F, ArgTypes...>
    {
    };

    template <class A> using result_of_t = typename result_of<A>::type;
    template <class A> using invoke_result_t = typename invoke_result<A>::type;
}

#endif

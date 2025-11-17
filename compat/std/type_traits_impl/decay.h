#ifndef IGRIS_STD_TYPE_TRAITS_DECAY_H
#define IGRIS_STD_TYPE_TRAITS_DECAY_H
#include "igris_std_namespace.h"

#include <type_traits_impl/is.h>
#include <type_traits_impl/standalone.h>

namespace igris_std
{
    namespace details
    {
        template <typename Base>
        igris_std::true_type is_base_of_test_func(const volatile Base *);
        template <typename Base>
        igris_std::false_type is_base_of_test_func(const volatile void *);
        template <typename Base, typename Derived>
        using pre_is_base_of =
            decltype(is_base_of_test_func<Base>(igris_std::declval<Derived *>()));

        // with <experimental/type_traits>:
        // template <typename Base, typename Derived>
        // using pre_is_base_of2 =
        // igris_std::experimental::detected_or_t<igris_std::true_type,
        // pre_is_base_of, Base, Derived>;
        template <typename Base, typename Derived, typename = void>
        struct pre_is_base_of2 : public igris_std::true_type
        {
        };
        // note igris_std::void_t is a C++17 feature
        template <typename Base, typename Derived>
        struct pre_is_base_of2<Base,
                               Derived,
                               igris_std::void_t<pre_is_base_of<Base, Derived>>>
            : public pre_is_base_of<Base, Derived>
        {
        };
    }

    template <typename Base, typename Derived>
    struct is_base_of
        : public igris_std::conditional_t<igris_std::is_class<Base>::value &&
                                        igris_std::is_class<Derived>::value,
                                    details::pre_is_base_of2<Base, Derived>,
                                    igris_std::false_type>
    {
    };

    template <class T> struct decay
    {
    private:
        typedef typename igris_std::remove_reference<T>::type U;

    public:
        typedef typename igris_std::conditional<
            igris_std::is_array<U>::value,
            typename igris_std::remove_extent<U>::type *,
            typename igris_std::conditional<igris_std::is_function<U>::value,
                                      typename igris_std::add_pointer<U>::type,
                                      typename igris_std::remove_cv<U>::type>::type>::
            type type;
    };
}

#endif

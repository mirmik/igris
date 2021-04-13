#ifndef IGRIS_DECLTYPEOF_H
#define IGRIS_DECLTYPEOF_H

//Макрос decltypeof имитирует поведение typeof в С/С++ коде.
#if __cplusplus
namespace
{
    template <class T> struct remove_reference
    {
        typedef T type;
    };
    template <class T> struct remove_reference<T &>
    {
        typedef T type;
    };
    template <class T> struct remove_reference<T &&>
    {
        typedef T type;
    };
}
#define decltypeof(a) typename remove_reference<decltype(a)>::type
#else
#define decltypeof(a) typeof(a)
#endif

#endif

#ifndef IGRIS_DECLTYPEOF_H
#define IGRIS_DECLTYPEOF_H

//Макрос decltypeof имитирует поведение typeof в С/С++ коде.

#if __cplusplus
#include <utility>
#define decltypeof(a) typename std::remove_reference<decltype(a)>::type
#else
#define decltypeof(a) typeof(a)
#endif

#endif

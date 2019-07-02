#ifndef IGRIS_MATH_DEG_H
#define IGRIS_MATH_DEG_H

#include <math.h>

template <typename T> constexpr T deg(T x) { return x * M_PI / 180; }
template <typename T> constexpr T deg2rad(T x) { return x * M_PI / 180; }

template <typename T> constexpr T rad2deg(T x) { return x * 180 / M_PI; }

#endif
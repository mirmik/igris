#ifndef IGRIS_MATH_DEG_H
#define IGRIS_MATH_DEG_H

#include <math.h>

static constexpr double deg(double x) { return x * M_PI / 180.0; }
static constexpr double deg2rad(double x) { return x * M_PI / 180.0; }

static constexpr double rad2deg(double x) { return x * 180.0 / M_PI; }

#endif
#ifndef IGRIS_MATH_DEG_H
#define IGRIS_MATH_DEG_H

#include <math.h>

#ifdef __cplusplus
static inline constexpr double deg(double x) { return x * M_PI / 180.0; }
static inline constexpr double deg2rad(double x) { return x * M_PI / 180.0; }
static inline constexpr double rad2deg(double x) { return x * 180.0 / M_PI; }
static inline constexpr float deg(float x) { return x * M_PI / 180.0; }
static inline constexpr float deg2rad(float x) { return x * M_PI / 180.0; }
static inline constexpr float rad2deg(float x) { return x * 180.0 / M_PI; }
#else
static inline double deg(double x) { return x * M_PI / 180.0; }
static inline double deg2rad(double x) { return x * M_PI / 180.0; }
static inline double rad2deg(double x) { return x * 180.0 / M_PI; }
static inline float deg(float x) { return x * M_PI / 180.0; }
static inline float deg2rad(float x) { return x * M_PI / 180.0; }
static inline float rad2deg(float x) { return x * 180.0 / M_PI; }
#endif

#define DEG2RAD (M_PI/180.0)
#define RAD2DEG (180.0/M_PI)

#endif

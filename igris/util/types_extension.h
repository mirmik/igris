#ifndef IGRIS_TYPES_EXTENSION_H
#define IGRIS_TYPES_EXTENSION_H

#include <stdint.h>

#ifndef IGRIS_FLOAT32
#	define IGRIS_FLOAT32
typedef float float32_t;
#endif

#ifndef WITHOUT_FLOAT64
#	ifndef IGRIS_FLOAT64
#		define IGRIS_FLOAT64
typedef double float64_t;
#	endif
#endif

#ifdef __cplusplus
static_assert(sizeof(float32_t) == 4);
#	ifndef WITHOUT_FLOAT64
static_assert(sizeof(float64_t) == 8);
#	endif
#else
_Static_assert(sizeof(float32_t) == 4);
#	ifndef WITHOUT_FLOAT64
_Static_assert(sizeof(float64_t) == 8);
#	endif
#endif

#endif
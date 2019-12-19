#ifndef IGRIS_UTIL_COMPILER_H
#define IGRIS_UTIL_COMPILER_H

#ifdef _MSC_VER
#define __NORETURN
#define __WEAK
#else
#define __NORETURN __attribute__((noreturn))
#define __WEAK __attribute__((weak))
#endif

#ifndef __BEGIN_DECLS
#	ifdef __cplusplus
#		define __BEGIN_DECLS extern "C" {
#	else
#		define __BEGIN_DECLS
#	endif
#endif

#ifndef __END_DECLS
#	ifdef __cplusplus
#		define __END_DECLS }
#	else
#		define __END_DECLS
#	endif
#endif

#define IS_ALIGNED(POINTER, BYTE_COUNT) \
    (((uintptr_t)(const void *)(POINTER)) % (BYTE_COUNT) == 0)

#endif
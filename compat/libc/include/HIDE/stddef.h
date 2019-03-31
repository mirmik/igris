/**
 * @file
 * @brief C standard library header.
 * @details Defines the macros #NULL and #offsetof as well
 *          as the types #ptrdiff_t and #size_t.
 *
 * @date 09.03.10
 * @author Eldar Abusalimov
 */

#ifndef STDDEF_H_
#define STDDEF_H_

#if __GNUC__ < 4 || defined(__CDT_PARSER__)
# define offsetof(type, member) \
	((size_t) &((type *) 0x0)->member)
#else
# define offsetof(type, member) \
	__builtin_offsetof(type, member)
#endif

#include <libc/defines/null.h>
#include <libc/defines/size_t.h>
#include <libc/defines/wchar_t.h>
#include <libc/defines/ptrdiff_t.h>

//FIXME remove off_t to sys/types.h 
//#include <libc/defines/off_t.h>

#endif /* STDDEF_H_ */

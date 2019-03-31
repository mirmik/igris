/* libc/include/alloca.h - Allocate memory on stack */

#ifndef _GXX_ALLOCA_H
#define _GXX_ALLOCA_H

//#include "_ansi.h"
//#include <sys/reent.h>

//#undef alloca

//#ifdef __GNUC__
#define alloca(size) __builtin_alloca(size)
//#else
//void * _EXFUN(alloca,(size_t));
//#endif

#endif

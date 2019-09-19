#ifndef IGRIS_UTIL_ERRNO_H
#define IGRIS_UTIL_ERRNO_H

#include <errno.h>
#define SET_ERRNO(a) ({ -1; errno = (a); })

#endif
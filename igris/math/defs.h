#ifndef IGRIS_MATH_DEFS_H
#define IGRIS_MATH_DEFS_H

#define __MAX__(a,b) ((a) > (b) ? (a) : (b))
#define __MIN__(a,b) ((a) < (b) ? (a) : (b))
#define __ABS__(a) ((a) < 0 ? (-a) : (a))

#ifndef MAX
#define MAX(a,b) __MAX__(a,b)
#endif

#ifndef MIN
#define MIN(a,b) __MIN__(a,b)
#endif

#ifndef ABS
#define ABS(a) __ABS__(a)
#endif

#endif
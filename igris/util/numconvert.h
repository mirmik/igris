#ifndef UTIL_NUMBER_CONVERT
#define UTIL_NUMBER_CONVERT

//Набор функций конвертеров в сишним стиле.

#include <igris/compiler.h>
#include <igris/util/types_extension.h>
#include <inttypes.h>
#include <stdlib.h>

__BEGIN_DECLS

char *i64toa(int64_t num, char *buf, uint8_t base);
char *i32toa(int32_t num, char *buf, uint8_t base);
char *i16toa(int16_t num, char *buf, uint8_t base);
char *i8toa(int8_t num, char *buf, uint8_t base);

char *u64toa(uint64_t num, char *buf, uint8_t base);
char *u32toa(uint64_t num, char *buf, uint8_t base);
char *u16toa(uint64_t num, char *buf, uint8_t base);
char *u8toa(uint64_t num, char *buf, uint8_t base);


IGRIS_DEPRECATED("Use strtoul instead")
static inline uint32_t atou32(const char *buf, uint8_t base, char **end)
{ return strtoul(buf, end, base); }

IGRIS_DEPRECATED("Use strtol instead")
static inline int32_t atoi32(const char *buf, uint8_t base, char **end)
{ return strtol(buf, end, base); }

#ifndef WITHOUT_ATOU64
IGRIS_DEPRECATED("Use strtoull instead")
static inline uint64_t atou64(const char *buf, uint8_t base, char **end)
{ return strtoull(buf, end, base); }
#endif

#ifndef WITHOUT_ATOI64
IGRIS_DEPRECATED("Use strtoll instead")
static inline int64_t atoi64(const char *buf, uint8_t base, char **end)
{ return strtoll(buf, end, base); }
#endif

IGRIS_DEPRECATED("Use strtof instead")
static inline float32_t atof32(const char *str, char **pend) { return (float)strtod(str, pend); } 
char *f32toa(float32_t f, char *buf, int8_t precision);

#ifndef WITHOUT_ATOF64
IGRIS_DEPRECATED("Use strtod instead")
static inline float64_t atof64(const char *str, char **pend) { return strtod(str, pend); } 
char *f64toa(float64_t f, char *buf, int8_t precision);
#endif

uint32_t igris_atou32(const char *buf, uint8_t base, char **end);
uint64_t igris_atou64(const char *buf, uint8_t base, char **end);
int32_t igris_atoi32(const char *buf, uint8_t base, char **end);
int64_t igris_atoi64(const char *buf, uint8_t base, char **end);

__END_DECLS

#endif

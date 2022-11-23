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

char *i64toa(int64_t num, char *buf, uint8_t base);
char *i32toa(int32_t num, char *buf, uint8_t base);
char *f32toa(float32_t f, char *buf, int8_t precision);
#ifndef WITHOUT_ATOF64
char *f64toa(float64_t f, char *buf, int8_t precision);
#endif

uint32_t igris_atou32(const char *buf, uint8_t base, char **end);
uint64_t igris_atou64(const char *buf, uint8_t base, char **end);
int32_t igris_atoi32(const char *buf, uint8_t base, char **end);
int64_t igris_atoi64(const char *buf, uint8_t base, char **end);
float32_t igris_atof32(const char *str, char **pend);
#ifndef WITHOUT_ATOF64
float64_t igris_atof64(const char *str, char **pend);
#endif

char *igris_i64toa(int64_t num, char *buf, uint8_t base);
char *igris_i32toa(int32_t num, char *buf, uint8_t base);
char *igris_f32toa(float32_t f, char *buf, int8_t precision);
#ifndef WITHOUT_ATOF64
char *igris_f64toa(float64_t f, char *buf, int8_t precision);
//#else
// char *igris_f64toa(float32_t f, char *buf, int8_t precision);
#endif

__END_DECLS

#endif

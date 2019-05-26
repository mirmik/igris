#ifndef UTIL_NUMBER_CONVERT
#define UTIL_NUMBER_CONVERT

//Набор функций конвертеров в сишним стиле.

#include <inttypes.h>
#include <igris/util/types_extension.h>

#include <igris/compiler.h>

__BEGIN_DECLS

char *i64toa( int64_t num, char *buf, uint8_t base );
char *i32toa( int32_t num, char *buf, uint8_t base );
char *i16toa( int16_t num, char *buf, uint8_t base );
char *i8toa( int8_t num, char *buf, uint8_t base );

char *u64toa( uint64_t num, char *buf, uint8_t base );
char *u32toa( uint64_t num, char *buf, uint8_t base );
char *u16toa( uint64_t num, char *buf, uint8_t base );
char *u8toa( uint64_t num, char *buf, uint8_t base );

uint32_t atou32(const char *buf, uint8_t base, char** end);
int32_t atoi32(const char *buf, uint8_t base, char** end);

float32_t atof32(const char* str, char** pend);
char * f32toa(float32_t f, char * buf, int8_t precision);

#ifndef WITHOUT_FLOAT64
float64_t atof64(const char* str, char** pend);
char * f64toa(float64_t f, char * buf, int8_t precision);
#endif

__END_DECLS

#endif

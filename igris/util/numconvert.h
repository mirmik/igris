#ifndef UTIL_NUMBER_CONVERT
#define UTIL_NUMBER_CONVERT

//Набор функций конвертеров в сишним стиле.

#include <sys/cdefs.h>
#include <inttypes.h>

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

char * ftoa(double f, char * buf, int precision);
double atod(const char* str, char** pend);

__END_DECLS

#endif

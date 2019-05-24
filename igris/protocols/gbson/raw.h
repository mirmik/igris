#ifndef IGRIS_PROTOCOLS_GBSON_RAW
#define IGRIS_PROTOCOLS_GBSON_RAW

#include <igris/util/types_extension.h>

#include <sys/cdefs.h>
#include <string.h>

#define GRAW_SIMPLE 0
#define GRAW_BUFFER 1
#define GRAW_ARRAY 2
#define GRAW_DICTIONARY 3

#define GRAW_ERROR_BUFSIZE -1
#define GRAW_ERROR_READOVER -2
#define GRAW_ERROR_WRITEOVER -3

__BEGIN_DECLS

static inline
int graw_read_simple(uint8_t **pp, uint8_t *endp, uint8_t* out, size_t typesize)
{
	if (endp - *pp < typesize)
		return GRAW_ERROR_READOVER;

	memcpy(out, *pp, typesize);
	*pp += typesize;

	return 0;
}

static inline
int graw_read_buffer(uint8_t **pp, uint8_t *endp, uint8_t* out, size_t outmax, size_t sizelen)
{
	size_t size;

	if (endp - *pp < sizelen)
		return GRAW_ERROR_READOVER;

	if (sizelen == 2)
	{
		uint16_t _size;
		memcpy(&_size, *pp, 2);
		*pp += 2;
		size = _size;
	}
	else if (sizelen == 4)
	{
		uint32_t _size;
		memcpy(&_size, *pp, 4);
		*pp += 4;
		size = _size;
	}
	else
		return GRAW_ERROR_BUFSIZE;

	if (endp - *pp < size)
		return GRAW_ERROR_READOVER;

	if (outmax < size)
		size = outmax;

	memcpy(out, *pp, size);
	*pp += size;

	return 0;
}

static inline
int graw_map_buffer(uint8_t **pp, uint8_t *endp, uint8_t** out, size_t* sizep, size_t sizelen)
{
	if (endp - *pp < sizelen)
		return GRAW_ERROR_READOVER;

	if (sizelen == 2)
	{
		uint16_t _size;
		memcpy(&_size, *pp, 2);
		*pp += 2;
		*sizep = _size;
	}
	else if (sizelen == 4)
	{
		uint32_t _size;
		memcpy(&_size, *pp, 4);
		*pp += 4;
		*sizep = _size;
	}
	else
		return GRAW_ERROR_BUFSIZE;

	if (endp - *pp < *sizep)
		return GRAW_ERROR_READOVER;

	*out = *pp;
	*pp += *sizep;

	return 0;
}

static inline
int graw_write_simple(uint8_t **pp, uint8_t *endp, uint8_t* src, size_t size) 
{
	if (endp - *pp < size)
		return GRAW_ERROR_WRITEOVER;	

	memcpy(*pp, src, size);
	*pp += size;

	return 0;
}

static inline int graw_write_i8(uint8_t **pp, uint8_t *endp, int8_t src) { return graw_write_simple(pp, endp, (uint8_t*)&src, sizeof(src)); }
static inline int graw_write_i16(uint8_t **pp, uint8_t *endp, int16_t src) { return graw_write_simple(pp, endp, (uint8_t*)&src, sizeof(src)); }
static inline int graw_write_i32(uint8_t **pp, uint8_t *endp, int32_t src) { return graw_write_simple(pp, endp, (uint8_t*)&src, sizeof(src)); }
static inline int graw_write_i64(uint8_t **pp, uint8_t *endp, int64_t src) { return graw_write_simple(pp, endp, (uint8_t*)&src, sizeof(src)); }

static inline int graw_write_u8(uint8_t **pp, uint8_t *endp, uint8_t src) { return graw_write_simple(pp, endp, (uint8_t*)&src, sizeof(src)); }
static inline int graw_write_u16(uint8_t **pp, uint8_t *endp, uint16_t src) { return graw_write_simple(pp, endp, (uint8_t*)&src, sizeof(src)); }
static inline int graw_write_u32(uint8_t **pp, uint8_t *endp, uint32_t src) { return graw_write_simple(pp, endp, (uint8_t*)&src, sizeof(src)); }
static inline int graw_write_u64(uint8_t **pp, uint8_t *endp, uint64_t src) { return graw_write_simple(pp, endp, (uint8_t*)&src, sizeof(src)); }

static inline int graw_write_f32(uint8_t **pp, uint8_t *endp, float32_t src) { return graw_write_simple(pp, endp, (uint8_t*)&src, sizeof(src)); }
static inline int graw_write_f64(uint8_t **pp, uint8_t *endp, float64_t src) { return graw_write_simple(pp, endp, (uint8_t*)&src, sizeof(src)); }

static inline int graw_read_i8(uint8_t **pp, uint8_t *endp, int8_t *dst) { return graw_read_simple(pp, endp, (uint8_t*)dst, sizeof(*dst)); }
static inline int graw_read_i16(uint8_t **pp, uint8_t *endp, int16_t *dst) { return graw_read_simple(pp, endp, (uint8_t*)dst, sizeof(*dst)); }
static inline int graw_read_i32(uint8_t **pp, uint8_t *endp, int32_t *dst) { return graw_read_simple(pp, endp, (uint8_t*)dst, sizeof(*dst)); }
static inline int graw_read_i64(uint8_t **pp, uint8_t *endp, int64_t *dst) { return graw_read_simple(pp, endp, (uint8_t*)dst, sizeof(*dst)); }

static inline int graw_read_u8(uint8_t **pp, uint8_t *endp, uint8_t *dst) { return graw_read_simple(pp, endp, (uint8_t*)dst, sizeof(*dst)); }
static inline int graw_read_u16(uint8_t **pp, uint8_t *endp, uint16_t *dst) { return graw_read_simple(pp, endp, (uint8_t*)dst, sizeof(*dst)); }
static inline int graw_read_u32(uint8_t **pp, uint8_t *endp, uint32_t *dst) { return graw_read_simple(pp, endp, (uint8_t*)dst, sizeof(*dst)); }
static inline int graw_read_u64(uint8_t **pp, uint8_t *endp, uint64_t *dst) { return graw_read_simple(pp, endp, (uint8_t*)dst, sizeof(*dst)); }

static inline int graw_read_f32(uint8_t **pp, uint8_t *endp, float32_t *dst) { return graw_read_simple(pp, endp, (uint8_t*)dst, sizeof(*dst)); }
static inline int graw_read_f64(uint8_t **pp, uint8_t *endp, float64_t *dst) { return graw_read_simple(pp, endp, (uint8_t*)dst, sizeof(*dst)); }

__END_DECLS

#endif
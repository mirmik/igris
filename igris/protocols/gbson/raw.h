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
	if (endp - *pp < (signed)typesize)
		return GRAW_ERROR_READOVER;

	memcpy(out, *pp, typesize);
	*pp += typesize;

	return 0;
}

static inline
int graw_read_buf(uint8_t **pp, uint8_t *endp, uint8_t* out, size_t outmax, uint16_t* sizep)
{
	int ans;

	ans = graw_read_simple(pp, endp, (uint8_t*)sizep, 2);
	if (ans)
		return ans;

	if (endp - *pp < *sizep)
		return GRAW_ERROR_READOVER;

	if (outmax < *sizep)
		*sizep = outmax;

	memcpy(out, *pp, *sizep);
	*pp += *sizep;

	return 0;
}

static inline
int graw_map_buf(uint8_t **pp, uint8_t *endp, uint8_t** out, uint16_t* sizep)
{
	int ans;

	ans = graw_read_simple(pp, endp, (uint8_t*)sizep, 2);
	if (ans)
		return ans;

	if (endp - *pp < *sizep)
		return GRAW_ERROR_READOVER;

	*out = *pp;
	*pp += *sizep;

	return 0;
}

static inline
int graw_read_longbuf(uint8_t **pp, uint8_t *endp, uint8_t* out, size_t outmax)
{
	uint32_t size;
	int ans;

	ans = graw_read_simple(pp, endp, (uint8_t*)&size, 4);
	if (ans)
		return ans;

	if (endp - *pp < size)
		return GRAW_ERROR_READOVER;

	if (outmax < size)
		size = outmax;

	memcpy(out, *pp, size);
	*pp += size;

	return 0;
}

static inline
int graw_map_longbuf(uint8_t **pp, uint8_t *endp, void** out, size_t* sizep)
{
	uint32_t size;
	int ans;

	ans = graw_read_simple(pp, endp, (uint8_t*)&size, 4);
	if (ans)
		return ans;

	if (endp - *pp < (signed)*sizep)
		return GRAW_ERROR_READOVER;

	*out = *pp;
	*pp += *sizep;

	return 0;
}

static inline
int graw_write_simple(uint8_t **pp, uint8_t *endp, uint8_t* src, size_t size) 
{
	if (endp - *pp < (signed)size)
		return GRAW_ERROR_WRITEOVER;	

	memcpy(*pp, src, size);
	*pp += size;

	return 0;
}

static inline
int graw_write_buf(uint8_t **pp, uint8_t *endp, uint8_t* src, uint16_t size) 
{
	int ans;

	ans = graw_write_simple(pp, endp, (uint8_t*)&size, 2);
	if (ans)
		return ans;

	ans = graw_write_simple(pp, endp, src, size);
	return ans;
}

static inline
int graw_write_longbuf(uint8_t **pp, uint8_t *endp, uint8_t* src, uint32_t size) 
{
	int ans;

	ans = graw_write_simple(pp, endp, (uint8_t*)&size, 4);
	if (ans)
		return ans;

	ans = graw_write_simple(pp, endp, src, size);
	return ans;
}

static inline
int graw_write_str(uint8_t **pp, uint8_t *endp, const char* src) 
{
	return graw_write_buf(pp, endp, (uint8_t*)src, strlen(src));
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
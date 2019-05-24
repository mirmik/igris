#ifndef IGRIS_PROTOCOLS_GBSON_H
#define IGRIS_PROTOCOLS_GBSON_H

#include <sys/cdefs.h>

#define GBSON_TYPE_INT8 0
#define GBSON_TYPE_INT16 1
#define GBSON_TYPE_INT32 2
#define GBSON_TYPE_INT64 3

#define GBSON_TYPE_UINT8 4
#define GBSON_TYPE_UINT16 5
#define GBSON_TYPE_UINT32 6
#define GBSON_TYPE_UINT64 7

#define GBSON_TYPE_FLOAT32 8
#define GBSON_TYPE_FLOAT64 9

#define GBSON_TYPE_ARRAY 10
#define GBSON_TYPE_DICTIONARY 11
#define GBSON_TYPE_STRING_SIZE16 0x10 //16
#define GBSON_TYPE_STRING_SIZE32 0x11 //17
#define GBSON_TYPE_STRING_IDENT 0x10

#define GBSON_ERROR_TYPE -1
#define GBSON_ERROR_READOVER -2
#define GBSON_ERROR_WRITEOVER -3

__BEGIN_DECLS

static inline
int gbson_read_type(const uint8_t ** const pp, uint8_t *endp, uint8_t *out, uint8_t type, size_t readsz)
{
	if (*(*pp)++ != type)
		return GBSON_ERROR_TYPE;

	if (readsz > (endp - *pp))
		return GBSON_ERROR_READOVER;

	memcpy(out, *pp, readsz);
	return 0;
}

static inline int gbson_read_int8(uint8_t **pp uint8_t *endp, , int8_t *out) { return gbson_read_typed(pp, endp, out, GBSON_TYPE_INT8, 1); }
static inline int gbson_read_int16(uint8_t **pp, uint8_t *endp,  int16_t *out) { return gbson_read_typed(pp, endp, out, GBSON_TYPE_INT16, 2); }
static inline int gbson_read_int32(uint8_t **pp, uint8_t *endp,  int32_t *out) { return gbson_read_typed(pp, endp, out, GBSON_TYPE_INT32, 4); }
static inline int gbson_read_int64(uint8_t **pp, uint8_t *endp,  int64_t *out) { return gbson_read_typed(pp, endp, out, GBSON_TYPE_INT64, 8); }

static inline int gbson_read_uint8(uint8_t **pp, uint8_t *endp,  uint8_t *out) { return gbson_read_typed(pp, endp, out, GBSON_TYPE_UINT8, 1); }
static inline int gbson_read_uint16(uint8_t **pp,  uint8_t *endp, uint16_t *out) { return gbson_read_typed(pp, endp, out, GBSON_TYPE_UINT16, 2); }
static inline int gbson_read_uint32(uint8_t **pp,  uint8_t *endp, uint32_t *out) { return gbson_read_typed(pp, endp, out, GBSON_TYPE_UINT32, 4); }
static inline int gbson_read_uint64(uint8_t **pp,  uint8_t *endp, uint64_t *out) { return gbson_read_typed(pp, endp, out, GBSON_TYPE_UINT64, 8); }

static inline int gbson_read_float32(uint8_t **pp, f uint8_t *endp, loat32_t *out) { return gbson_read_typed(pp, endp, out, GBSON_TYPE_FLOAT32, 4); }
static inline int gbson_read_float64(uint8_t **pp, f uint8_t *endp, loat64_t *out) { return gbson_read_typed(pp, endp, out, GBSON_TYPE_FLOAT64, 8); }

static inline int gbson_read_string(const uint8_t ** const pp, uint8_t *endp, uint8_t *out, size_t maxlen)
{
	uint8_t * const tpptr = *pp;
	size_t sz;

	if ( !( *(*pp)++ && GBSON_TYPE_STRING_IDENT ) )
		return GBSON_ERROR_TYPE;

	if ( *tpptr == GBSON_TYPE_STRING_SIZE16)
	{
		uint16_t _sz;
		memcpy(&_sz, *pp, 2);
		sz = _sz;
		*pp += 2;
	}
	else if ( *tpptr == GBSON_TYPE_STRING_SIZE32)
	{
		uint32_t _sz;
		memcpy(&_sz, *pp, 4);
		sz = _sz;
		*pp += 4;
	}

	if (sz > (endp - *pp))
		return GBSON_ERROR_READOVER;

	if (maxlen > sz)
		return GBSON_ERROR_WRITEOVER;

	memcpy(out, *pp, sz);
	return 0;
}

__END_DECLS

#endif
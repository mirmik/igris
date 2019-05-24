#ifndef IGRIS_PROTOCOLS_MSGTYPE_H
#define IGRIS_PROTOCOLS_MSGTYPE_H

#include <igris/gbson/raw.h>

#define GRAW_SIMPLE 0
#define GRAW_BUFFER 1
#define GRAW_ARRAY 2
#define GRAW_DICTIONARY 3

namespace igris
{
	class msgtype
	{
		std::string name;
		int type;
		int size;
	};

	const msgtype msgtype_i8 =  { "i8",  GRAW_SIMPLE, 1 }; 
	const msgtype msgtype_i16 = { "i16", GRAW_SIMPLE, 2 }; 
	const msgtype msgtype_i32 = { "i32", GRAW_SIMPLE, 4 }; 
	const msgtype msgtype_i64 = { "i64", GRAW_SIMPLE, 8 };

	const msgtype msgtype_u8 =  { "u8",  GRAW_SIMPLE, 1 }; 
	const msgtype msgtype_u16 = { "u16", GRAW_SIMPLE, 2 }; 
	const msgtype msgtype_u32 = { "u32", GRAW_SIMPLE, 4 }; 
	const msgtype msgtype_u64 = { "u64", GRAW_SIMPLE, 8 };

	const msgtype msgtype_f32 = { "f32", GRAW_SIMPLE, 4 }; 
	const msgtype msgtype_f64 = { "f64", GRAW_SIMPLE, 8 };

	const msgtype msgtype_str =     { "str",     GRAW_BUFFER, 2 };	
	const msgtype msgtype_longstr = { "longstr", GRAW_BUFFER, 4 };

	class msgtype_map
	{
		std::vector<uint8_t*> ptrs;
	};

	class msgtype_reader
	{
		std::vector<const msgtype&, std::string> tstruct;
		msgtype_map map(uint8_t* strt, uint8_t* fini);
	};

	msgtype_reader readtype(const std::string& typename, const std::string& filepath);

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
	int graw_map_buffer(uint8_t **pp, uint8_t *endp, uint8_t** out, size_t* size, size_t sizelen) 
	{
		if (endp - *pp < sizelen)
			return GRAW_ERROR_READOVER;

		if (sizelen == 2) 
		{
			uint16_t _size;
			memcpy(&_size, *pp, 2);
			*pp += 2;
			*size = _size;
		} 
		else if (sizelen == 4) 
		{
			uint32_t _size;
			memcpy(&_size, *pp, 4);
			*pp += 4;
			*size = _size;	
		}
		else 
			return GRAW_ERROR_BUFSIZE;

		if (endp - *pp < size)
			return GRAW_ERROR_READOVER;

		*out = *pp;
		*pp += *size;

		return 0;
	}
}

#endif
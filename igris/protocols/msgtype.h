#ifndef IGRIS_PROTOCOLS_MSGTYPE_H
#define IGRIS_PROTOCOLS_MSGTYPE_H

#include <igris/protocols/gbson/raw.h>

#include <string>
#include <vector>

namespace igris
{
	struct msgtype
	{
		std::string name;
		uint8_t type;
		uint8_t size;

		msgtype(const std::string& str, uint8_t type, uint8_t size):
			name(str), type(type), size(size) {}
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
		std::vector<msgtype> tstruct;
		msgtype_map map(uint8_t* strt, uint8_t* fini);
	};

	msgtype_reader readtype(const std::string& type, const std::string& filepath);
}

#endif
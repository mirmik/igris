#ifndef IGRIS_PROTOCOLS_MSGTYPE_H
#define IGRIS_PROTOCOLS_MSGTYPE_H

#include <igris/protocols/gbson/raw.h>
#include <igris/buffer.h>

#include <assert.h>

#include <iostream>
#include <string>
#include <vector>
#include <map>

namespace igris
{
	struct msgtype
	{
		uint8_t type;
		uint8_t size;
		std::string(*tostring)(uint8_t* strt, uint8_t* fini);

		msgtype() = default;
		msgtype(uint8_t type, uint8_t size, std::string(*tostring)(uint8_t* strt, uint8_t* fini)):
			type(type), size(size), tostring(tostring) {}
	};

	template<class T>
	std::string msgtype_tostring_simple(uint8_t* strt, uint8_t* fini) 
	{
		assert(sizeof(T) == fini - strt);

		T val;
		memcpy((void*)&val, strt, sizeof(T));

		return std::to_string(val);
	}

	static
	std::string msgtype_tostring_buffer(uint8_t* strt, uint8_t* fini) 
	{
		return std::string((char*)strt, fini - strt);
	}

	const msgtype msgtype_i8 =  { GRAW_SIMPLE, 1, msgtype_tostring_simple<int8_t> }; 
	const msgtype msgtype_i16 = { GRAW_SIMPLE, 2, msgtype_tostring_simple<int16_t> }; 
	const msgtype msgtype_i32 = { GRAW_SIMPLE, 4, msgtype_tostring_simple<int32_t> }; 
	const msgtype msgtype_i64 = { GRAW_SIMPLE, 8, msgtype_tostring_simple<int64_t> };

	const msgtype msgtype_u8 =  { GRAW_SIMPLE, 1, msgtype_tostring_simple<uint8_t> }; 
	const msgtype msgtype_u16 = { GRAW_SIMPLE, 2, msgtype_tostring_simple<uint16_t> }; 
	const msgtype msgtype_u32 = { GRAW_SIMPLE, 4, msgtype_tostring_simple<uint32_t> }; 
	const msgtype msgtype_u64 = { GRAW_SIMPLE, 8, msgtype_tostring_simple<uint64_t> };

	const msgtype msgtype_f32 = { GRAW_SIMPLE, 4, msgtype_tostring_simple<float32_t> }; 
	const msgtype msgtype_f64 = { GRAW_SIMPLE, 8, msgtype_tostring_simple<float64_t> };

	const msgtype msgtype_str =     { GRAW_BUFFER, 2, msgtype_tostring_buffer };	
	const msgtype msgtype_longstr = { GRAW_BUFFER, 4, msgtype_tostring_buffer };

	struct msgtype_map
	{
		std::vector<uint8_t*> ptrs;
		bool success;
	};

	struct msgtype_reader
	{
		std::vector<std::pair<std::string, msgtype>> tstruct;
		
		msgtype_map map(uint8_t* strt, uint8_t* fini) 
		{
			msgtype_map m;
			uint8_t* ptr = strt;

			m.ptrs.push_back(ptr);

			for (auto t: tstruct) 
			{
				const msgtype& msgtp = t.second;

				if (msgtp.type == GRAW_SIMPLE) 
					ptr += msgtp.size;

				else if(msgtp.type == GRAW_BUFFER) 
				{
					assert(msgtp.size == 2);

					uint16_t size;
					memcpy(&size, ptr, 2);
					ptr += 2;
					ptr += size;										
				}

				m.ptrs.push_back(ptr);
			}

			if (fini == ptr)
				m.success = true;
			else
				m.success = false;

			return m;
		}

		std::vector<std::pair<std::string,std::string>> 
		tostring(igris::buffer data) 
		{
			msgtype_map m = map((uint8_t*)data.data(), (uint8_t*)data.data() + data.size());			
			return tostring(m);
		}

		std::vector<std::pair<std::string,std::string>>
		tostring(const msgtype_map& m) 
		{
			std::vector<std::pair<std::string,std::string>> ret;
			
			for(int i = 0; i < tstruct.size(); ++i) 
			{
				auto str1 = tstruct[i].first.data();
				auto str2 = tstruct[i].second.tostring(m.ptrs[i], m.ptrs[i+1]);
				ret.push_back(std::make_pair(str1, str2));
			}

			return ret;
		}
	};

	msgtype_reader msgtype_read_type(const std::string& type, const std::string& filepath);
}

#endif
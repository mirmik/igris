#ifndef IGRIS_PROTOCOLS_MSGTYPE_H
#define IGRIS_PROTOCOLS_MSGTYPE_H

#include <igris/protocols/gbson/raw.h>
#include <igris/util/string.h>
#include <igris/buffer.h>
#include <igris/dprint.h>
#include <igris/util/bug.h>

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
		void(*fromstring)(const std::string & str, uint8_t** buf);

		msgtype() = default;
		msgtype(uint8_t type, uint8_t size, 
			std::string(*tostring)(uint8_t* strt, uint8_t* fini),
			void(*fromstring)(const std::string &, uint8_t**))
		:
			type(type), size(size), tostring(tostring), fromstring(fromstring) {}

		std::string fromstring_as_string(const std::string& str) 
		{
			std::string ret;

			switch (type) 
			{
				case GRAW_SIMPLE: {
					ret.resize(size);
					uint8_t * p = (uint8_t*)ret.data();
					fromstring(str, &p);
					return ret;
				}
				case GRAW_BUFFER: {
					BUG();
					ret.resize(size);
					uint8_t * p = (uint8_t*)ret.data();
					fromstring(str, &p);
					return ret;
				}
				default:
					BUG();
			}		
		}
	};

	template<class T>
	std::string msgtype_tostring_simple(uint8_t* strt, uint8_t* fini) 
	{
		assert(sizeof(T) == fini - strt);

		T val;
		memcpy((void*)&val, strt, sizeof(T));

		return std::to_string(val);
	}

	template<class T>
	void msgtype_fromstring_integer(const std::string& str, uint8_t** pp) 
	{
		T val = std::stoi(str);
		memcpy(*pp, &val, sizeof(T));
		*pp += sizeof(T);
	}

	template<class T>
	void msgtype_fromstring_float(const std::string& str, uint8_t** pp) 
	{
		T val = std::stof(str);
		memcpy(*pp, &val, sizeof(T));
		*pp += sizeof(T);
	}

	static
	void msgtype_fromstring_symbol(const std::string& str, uint8_t** pp) 
	{
		//Добавить поле размера.
		BUG();
		uint16_t size = str.size();
		memcpy(*pp, str.data(), size);
		*pp += size;
	}

	static
	std::string msgtype_tostring_buffer(uint8_t* strt, uint8_t* fini) 
	{
		return std::string((char*)strt, fini - strt);
	}

	const msgtype msgtype_i8 =  { GRAW_SIMPLE, 1, msgtype_tostring_simple<int8_t>, msgtype_fromstring_integer<int8_t> }; 
	const msgtype msgtype_i16 = { GRAW_SIMPLE, 2, msgtype_tostring_simple<int16_t>, msgtype_fromstring_integer<int16_t> }; 
	const msgtype msgtype_i32 = { GRAW_SIMPLE, 4, msgtype_tostring_simple<int32_t>, msgtype_fromstring_integer<int32_t> }; 
	const msgtype msgtype_i64 = { GRAW_SIMPLE, 8, msgtype_tostring_simple<int64_t>, msgtype_fromstring_integer<int64_t> };

	const msgtype msgtype_u8 =  { GRAW_SIMPLE, 1, msgtype_tostring_simple<uint8_t>, msgtype_fromstring_integer<uint8_t> }; 
	const msgtype msgtype_u16 = { GRAW_SIMPLE, 2, msgtype_tostring_simple<uint16_t>, msgtype_fromstring_integer<uint16_t> }; 
	const msgtype msgtype_u32 = { GRAW_SIMPLE, 4, msgtype_tostring_simple<uint32_t>, msgtype_fromstring_integer<uint32_t> }; 
	const msgtype msgtype_u64 = { GRAW_SIMPLE, 8, msgtype_tostring_simple<uint64_t>, msgtype_fromstring_integer<uint64_t> };

	const msgtype msgtype_f32 = { GRAW_SIMPLE, 4, msgtype_tostring_simple<float32_t>, msgtype_fromstring_float<float32_t> }; 
	const msgtype msgtype_f64 = { GRAW_SIMPLE, 8, msgtype_tostring_simple<float64_t>, msgtype_fromstring_float<float64_t> };

	const msgtype msgtype_str =     { GRAW_BUFFER, 2, msgtype_tostring_buffer, msgtype_fromstring_symbol };	
	const msgtype msgtype_longstr = { GRAW_BUFFER, 4, msgtype_tostring_buffer, msgtype_fromstring_symbol };

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
			
			for(unsigned i = 0; i < tstruct.size(); ++i) 
			{
				auto str1 = tstruct[i].first.data();
				auto str2 = tstruct[i].second.tostring(m.ptrs[i], m.ptrs[i+1]);
				ret.push_back(std::make_pair(str1, str2));
			}

			return ret;
		}

		std::string input(const std::string & str) 
		{
			std::string ret;
			igris::strvec tokens = igris::split(str, ' ');

			if (tokens.size() != tstruct.size()) 
			{
				dprln("Warn: wrong number of tokens.");
				return "";
			}

			for (unsigned int i=0; i < tokens.size(); ++i) 
			{
				ret += tstruct[i].second.fromstring_as_string(tokens[i]); 
			}

			return ret;
		}
	};

	msgtype_reader msgtype_read_type(const std::string& type, const std::string& filepath);
}

#endif
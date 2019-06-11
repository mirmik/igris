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

#include <nos/print.h>
#include <nos/fprint.h>

namespace igris
{
	/*	struct msgtype
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
		};*/

	struct msgtype
	{
		virtual std::string tostring(uint8_t* strt, uint8_t* fini) = 0;
		virtual void fromstring(const std::string & str, uint8_t** pp) = 0;
		virtual size_t size(uint8_t** pp) = 0;

		virtual ssize_t print_to(nos::ostream & os) const = 0;

		std::string fromstring_as_string(const std::string& str)
		{
			std::string ret;
			uint8_t * p_ = (uint8_t*)str.data();

			ret.resize(size(&p_));
			uint8_t * p = (uint8_t*)ret.data();
			fromstring(str, &p);

			return ret;
		}

		std::string tostring(igris::buffer buf) 
		{ 
			return tostring(
				(uint8_t*)buf.data(), 
				(uint8_t*)buf.data() + buf.size()); 
		}
	};

	template <class T>
	struct msgtype_integer : public msgtype
	{
		std::string tostring(uint8_t* strt, uint8_t* fini) override
		{
			assert(sizeof(T) == fini - strt);
			T val;
			memcpy((void*)&val, strt, sizeof(T));
			return std::to_string(val);
		}

		void fromstring(const std::string & str, uint8_t** pp) override
		{
			T val = std::stoi(str);
			memcpy(*pp, &val, sizeof(T));
			*pp += sizeof(T);
		}

		size_t size(uint8_t** pp) override 
		{ 
			*pp += sizeof(T);
			return sizeof(T); 
		}

		static std::unique_ptr<msgtype> make_unique() 
		{ return std::make_unique<msgtype_integer>(); }

		ssize_t print_to(nos::ostream & os) const override
		{
			return nos::fprint_to(os, "i{}", sizeof(T)*8);
		}
	};

	template <class T>
	struct msgtype_float : public msgtype
	{
		std::string tostring(uint8_t* strt, uint8_t* fini) override
		{
			assert(sizeof(T) == fini - strt);
			T val;
			memcpy((void*)&val, strt, sizeof(T));
			return std::to_string(val);
		}

		void fromstring(const std::string & str, uint8_t** pp) override
		{
			T val = std::stof(str);
			memcpy(*pp, &val, sizeof(T));
			*pp += sizeof(T);
		}

		size_t size(uint8_t** pp) override 
		{ 
			*pp += sizeof(T);
			return sizeof(T); 
		}

		static std::unique_ptr<msgtype> make_unique() 
		{ return std::make_unique<msgtype_float>(); }

		ssize_t print_to(nos::ostream & os) const override
		{
			return nos::fprint_to(os, "f{}", sizeof(T)*8);
		}
	};

	struct msgtype_string : public msgtype
	{
		int sizelen;

		msgtype_string(int sizelen) : sizelen(sizelen) {}

		std::string tostring(uint8_t* strt, uint8_t* fini) override
		{
			return std::string((char*)strt, fini - strt);
		}

		void fromstring(const std::string & str, uint8_t** pp) override
		{
			//Добавить поле размера.
			BUG();
			uint16_t size = str.size();
			memcpy(*pp, str.data(), size);
			*pp += size;
		}

		size_t size(uint8_t ** pp) override { (void)pp; BUG(); return 0; }

		ssize_t print_to(nos::ostream & os) const override
		{
			if (sizelen == 2)
				return nos::print_to(os, "str");
			else
				return nos::print_to(os, "longstr");
		}
	};

	const msgtype_integer<int8_t> msgtype_i8;
	const msgtype_integer<int16_t> msgtype_i16;
	const msgtype_integer<int32_t> msgtype_i32;
	const msgtype_integer<int64_t> msgtype_i64;

	const msgtype_integer<uint8_t> msgtype_u8;
	const msgtype_integer<uint16_t> msgtype_u16;
	const msgtype_integer<uint32_t> msgtype_u32;
	const msgtype_integer<uint64_t> msgtype_u64;

	const msgtype_integer<float32_t> msgtype_f32;
	const msgtype_integer<float64_t> msgtype_f64;

	const msgtype_string msgtype_str(2);
	const msgtype_string msgtype_longstr(4);

	/*const msgtype msgtype_i16 = { GRAW_SIMPLE, 2, msgtype_tostring_simple<int16_t>, msgtype_fromstring_integer<int16_t> };
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
*/
	extern std::map<std::string, msgtype*> base_msgtypes;




	struct msgtype_struct  : public msgtype
	{
		using pair_t = std::pair<std::string, std::unique_ptr<msgtype>>;
		std::vector<pair_t> objects;

		msgtype_struct(){}

		int readtype(const std::string & typestr);

		ssize_t print_to(nos::ostream& os) const override
		{
			ssize_t ret = 0;
			for (auto& a : objects) 
			{
				ret += nos::fprintln_to(os, "{}:{}", *a.second, a.first);
			}
			return ret;
		}

		std::string tostring(uint8_t* strt, uint8_t* fini)  override { (void)(strt);(void)fini; BUG(); return ""; }
		using msgtype::tostring;

		void fromstring(const std::string & str, uint8_t** buf) override 
		{ 
			uint8_t * p; 

			auto v = igris::split(str);

			BUG();

			for (auto& m : objects) 
			{
				ret += m.second->fromstring(str, &p);
			}
		}

		size_t size(uint8_t ** pp) override 
		{
			size_t ret = 0;
			for (auto& m : objects) 
			{
				ret += m.second->size(pp);
			}
			return ret;
		}
	};


	struct msgtype_array : public msgtype
	{
		std::unique_ptr<msgtype> derived;		

		msgtype_array(std::unique_ptr<msgtype>&& ptr) : derived(std::move(ptr)) {}

		std::string tostring(uint8_t* strt, uint8_t* fini)  override { (void)(strt);(void)fini; BUG(); return ""; }

		void fromstring(const std::string & str, uint8_t** buf) override { (void)(buf);(void)str; BUG();}

		size_t size(uint8_t** pp) override {(void)pp; BUG();}

		ssize_t print_to(nos::ostream & os) const override 
		{
			int ret = 0;
			ret += nos::print_to(os, *derived);
			ret += nos::print_to(os, "[]");
			return ret;
		}
	};










	struct msgtype_map
	{
		std::vector<uint8_t*> ptrs;
		bool success;
	};

/*	struct msgtype_reader
	{
		std::vector<std::pair<std::string, msgtype*>> tstruct;

		msgtype_map map(uint8_t* strt, uint8_t* fini)
		{
			msgtype_map m;
			uint8_t* ptr = strt;

			m.ptrs.push_back(ptr);

			for (auto t : tstruct)
			{
				const msgtype& msgtp = t.second;

				if (msgtp.type == GRAW_SIMPLE)
					ptr += msgtp.size();

				else if (msgtp.type == GRAW_BUFFER)
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

			return m;*/
/*		}

		std::vector<std::pair<std::string, std::string>>
		        tostring(igris::buffer data)
		{
			msgtype_map m = map((uint8_t*)data.data(), (uint8_t*)data.data() + data.size());
			return tostring(m);
		}

		std::vector<std::pair<std::string, std::string>>
		        tostring(const msgtype_map& m)
		{
			std::vector<std::pair<std::string, std::string>> ret;

			for (unsigned i = 0; i < tstruct.size(); ++i)
			{
				auto str1 = tstruct[i].first.data();
				auto str2 = tstruct[i].second->tostring(m.ptrs[i], m.ptrs[i + 1]);
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

			for (unsigned int i = 0; i < tokens.size(); ++i)
			{
				ret += tstruct[i].second->fromstring_as_string(tokens[i]);
			}

			return ret;
		}
	};*/

	msgtype_struct msgtype_read_type(const std::string& type, const std::string& filepath);

	/*struct msgtype_object
	{
		std::string type;
		std::string name;
		std::string mod;

		msgtype_object(
		    const std::string& a,
		    const std::string& b,
		    const std::string& c) : type(a), name(b), mod(c) {}

		ssize_t print_to(nos::ostream& os) const
		{
			return nos::fprint_to(os, "[a:{},b:{},c:{}]", type, name, mod);
		}
	};*/

	struct msgtype_object
	{
		msgtype * oper;
		
	};

	std::vector<msgtype_object> type_objectize(const std::string& str);
}

#endif
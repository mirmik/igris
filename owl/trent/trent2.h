#ifndef GXX_TRENT_H
#define GXX_TRENT_H

#include <gxx/print.h>

#include <memory>
#include <gxx/panic.h>
#include <gxx/util/bits.h>
#include <gxx/container/dlist.h>
#include <gxx/datastruct/buffer_head.h>

static constexpr uint8_t TRENT_ALLOC = 0x80;
static constexpr uint8_t TRENT_ALLOC_NAME = 0x40;
static constexpr uint8_t TRENT_ALLOC_BUFFER = 0x20;

#include <gxx/trace.h>

namespace gxx
{
	class trent2
	{
		enum Type
		{
			Nil = 0x00,
			Bool = 0x01,
			Numer = 0x02,
			String = 0x03,
			Table = 0x10,
			List = 0x11,
			Dict = 0x12,
		};

		struct trent * parent = nullptr;
		struct dlist_head lnk;
		struct buffer_head name;

		uint8_t flags = 0;
		uint8_t type = Type::Nil;
		
		union
		{
			double num;
			struct buffer_head buf;
			struct dlist_head tbl;
		} data;

	public:
		trent2()	
		{
			TRACE();
		}

		trent2(double num) : trent2()		
		{
			TRACE();
			init(num);
		}

		~trent2()
		{
			TRACE();
			invalidate();
		}

		void dealloc(void* ptr) {
			TRACE();
			free(ptr);
		}

		void invalidate()
		{
			TRACE();
			switch (type)
			{
				case Type::Nil:
				case Type::Numer:
				case Type::Bool:
					return;

				case Type::String:
					if (flags & TRENT_ALLOC_BUFFER){}
						dealloc(data.buf.data);
					return;

				case List:
				case Dict:
				case Table:
				{
					while (!dlist_empty(&data.tbl)) 
					{
						trent2* ptr = dlist_first_entry(&data.tbl, trent2, lnk);
						ptr->invalidate();
						delete ptr;
					}
				}

				default:
					PANIC_TRACED();
			}
			type = Type::Nil;
		}

		bool is_table() { return type & 0x10; }
		bool is_list() { return type == Type::List; }
		bool is_dict() { return type == Type::Dict; }

		void init(double num) 
		{
			TRACE();
			type = Type::Numer;
			data.num = num;
		}

		void init(int num) 
		{
			TRACE();
			type = Type::Numer;
			data.num = num;
		}

		void init(bool b) 
		{
			TRACE();
			type = Type::Bool;
			data.num = b ? 1 : 0;
		}

		void init(const char * str, size_t size) 
		{
			type = Type::String;
			bits_set(flags, TRENT_ALLOC_BUFFER);
			data.buf.size = size;
			data.buf.data = malloc(size);
			memcpy(data.buf.data, str, size);
		}

		void init(const std::string& str) 
		{
			init(str.data(), str.size());
		}

		void init(const char * str) 
		{
			init(str, strlen(str));
		}

		template <typename T>
		trent2& operator= (T&& obj)
		{
			TRACE();
			invalidate();
			init(obj);
			return *this;
		}

/*		size_t printTo(gxx::io::ostream& os) const
		{
			TRACE();
			bool sep = false;
			switch (type)
			{
				case Type::Bool:
					os.print(data.num != 0 ? "true" : "false");
					return 0;

				case Type::Numer:
					os.print(data.num);
					return 0;

				case Type::String:
				//	os.putchar('"');
				//	gxx::print_to(os, unsafe_string_const());
				//	os.putchar('"');
					return 0;
				
				/*case trent::type::list:
					os.putchar('[');
					for (auto& v : unsafe_list_const())
					{
						if (sep) os.putchar(',');
						v.printTo(os);
						sep = true;
					}
					os.putchar(']');
					return 0;
				
				case trent::type::dict:
					os.putchar('{');
					for (auto& p : unsafe_dict_const())
					{
						if (sep) os.putchar(',');
						os.putchar('"');
						gxx::print_to(os, p.first);
						os.putchar('"');
						os.putchar(':');
						p.second.printTo(os);
						sep = true;
					}
					os.putchar('}');
					return 0;*/
/*				case Type::Nil:
					os.print("nil");
					return 0;

				default:
					PANIC_TRACED();
			}
		}*/
	};
}

#endif
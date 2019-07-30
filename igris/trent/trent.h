#ifndef GXX_DATATREE_H
#define GXX_DATATREE_H

#include <cstdint>
#include <cassert>
#include <utility>

#include <string>
#include <vector>
#include <map>

#include <igris/util/ctrdtr.h>
#include <igris/util/bug.h>

namespace igris
{
	template <template <class Allocator> class TAlloc>
	class trent_basic
	{
	public:
		//SPECS

		using valloc_t = std::allocator<trent_basic>;
		using malloc_t = std::allocator<std::pair<std::string, trent_basic>>;

		enum class type : uint8_t
		{
			string,
			list,
			dict,
			numer,
			integer,
			boolean,
			nil,
		};

		using numer_type = long double;
		using integer_type = int64_t;
		using list_type = std::vector<trent_basic, valloc_t>;
		using dict_type = std::map<std::string, trent_basic, malloc_t>;
		using string_type = std::string;

	protected:
		// FIELDS

		type m_type = type::nil;
		union
		{
			numer_type m_num;
			integer_type m_int;
			list_type m_arr;
			dict_type m_dict;
			string_type m_str;
		};

	public:
		~trent_basic();
		trent_basic() : m_type(type::nil) {}
		trent_basic(const trent_basic& other);

		void invalidate();

		/*	inline trent(const std::string& str) { init(str); }
			inline trent(const char* str) { init(str); }
			inline trent(const trent::type& t) { init(t); }
			inline trent(const float& i) { init(i); }
			inline trent(const double& i) { init(i); }
			inline trent(const long double& i) { init(i); }
			inline trent(const signed char& i) { init(i); }
			inline trent(const signed short& i) { init(i); }
			inline trent(const signed int& i) { init(i); }
			inline trent(const signed long& i) { init(i); }
			inline trent(const signed long long& i) { init(i); }
			inline trent(const unsigned char& i) { init(i); }
			inline trent(const unsigned short& i) { init(i); }
			inline trent(const unsigned int& i) { init(i); }
			inline trent(const unsigned long& i) { init(i); }
			inline trent(const unsigned long long& i) { init(i); }
			inline trent(const bool& i) { init(i); }

		public:
			void init(trent::type t);
			void init(const std::string& str);
			void init(gxx::buffer buf);
			void init(const char* str);
			void init(const float& i);
			void init(const double& i);
			void init(const long double& i);
			void init(const signed char& i);
			void init(const signed short& i);
			void init(const signed int& i);
			void init(const signed long& i);
			void init(const signed long long& i);
			void init(const unsigned char& i);
			void init(const unsigned short& i);
			void init(const unsigned int& i);
			void init(const unsigned long& i);
			void init(const unsigned long long& i);
			void init(const bool& i);

			template <typename T>
			void reset(T obj)
			{
				invalidate();
				init(obj);
			}

			void init_list(size_t reserve);

		public:
			const trent& operator[](int i) const;
			const trent& operator[](const char* key) const;
			const trent& operator[](const std::string& key) const;
			const trent& operator[](const gxx::buffer& key) const;
			const trent& operator[](const trent_path& path) const;

			trent& operator[](int i);
			trent& operator[](const char* key);
			trent& operator[](const std::string& key);
			trent& operator[](const gxx::buffer& key);
			trent& operator[](const trent_path& path);

			const trent& at(int i) const;
			const trent& at(const char* key) const;
			const trent& at(const std::string& key) const;
			const trent& at(const gxx::buffer& key) const;
			const trent& at(const trent_path& path) const;

			trent& at(int i);
			trent& at(const char* key);
			trent& at(const std::string& key);
			trent& at(const gxx::buffer& key);
			trent& at(const trent_path& path);

			bool have(const std::string& key) const;

			std::map<std::string, trent>& as_dict();
			const std::map<std::string, trent>& as_dict() const;
			result<std::map<std::string, trent>&> as_dict_critical();
			result<const std::map<std::string, trent>&> as_dict_critical() const;

			std::vector<trent>& as_list();
			const std::vector<trent>& as_list() const;

			result<std::vector<trent>&> as_list_critical();
			result<const std::vector<trent>&> as_list_critical() const;

			numer_type as_numer() const;
			numer_type as_numer_default(numer_type i) const;
			result<numer_type> as_numer_critical() const;

			integer_type as_integer() const;
			integer_type as_integer_default(integer_type i) const;
			result<integer_type> as_integer_critical() const;

			string_type& as_string();
			const string_type& as_string() const;
			const gxx::buffer as_buffer() const;
			string_type& as_string_default(string_type& str);
			result<string_type&> as_string_critical();
			result<const string_type&> as_string_critical() const;

			integer_type& unsafe_integer_const() { return m_int; }
			numer_type& unsafe_numer_const() { return m_num; }
			string_type& unsafe_string_const() { return m_str; }
			list_type& unsafe_list_const() { return m_arr; }
			dict_type& unsafe_dict_const() { return m_dict; }

			const integer_type& unsafe_integer_const() const { return m_int; }
			const numer_type& unsafe_numer_const() const { return m_num; }
			const string_type& unsafe_string_const() const { return m_str; }
			const list_type& unsafe_list_const() const { return m_arr; }
			const dict_type& unsafe_dict_const() const { return m_dict; }
			const integer_type& unsafe_bool_const() const { return m_int; }

			trent::type get_type() const;
			const char * type_to_str() const;

			bool is_nil() const 		{ return m_type == type::nil; }
			bool is_bool() const 		{ return m_type == type::boolean; }
			bool is_numer() const 		{ return m_type == type::numer || m_type == type::integer; }
			bool is_integer() const     { return m_type == type::integer; }
			bool is_list() const		{ return m_type == type::list; }
			bool is_dict() const        { return m_type == type::dict; }
			bool is_string() const 		{ return m_type == type::string; }

			//strlst check_dict(strlst lst, check_type ct);
			//std::pair<strlst,strlst> check_dict_symmetric(strlst lst);

		public:
			trent& operator= (const trent& other);
			trent& operator= (const std::string& str);
			trent& operator= (gxx::buffer buf);

			trent& operator= (float num);
			trent& operator= (double num);
			trent& operator= (long double num);

			trent& operator= (signed char i);
			trent& operator= (signed short i);
			trent& operator= (signed int i);
			trent& operator= (signed long i);
			trent& operator= (signed long long i);
			trent& operator= (unsigned char i);
			trent& operator= (unsigned short i);
			trent& operator= (unsigned int i);
			trent& operator= (unsigned long i);
			trent& operator= (unsigned long long i);
			trent& operator= (bool i);
			ssize_t size();

			bool contains(gxx::buffer buf);

			size_t printTo(gxx::io::ostream& os) const
			{
				bool sep = false;
				switch (get_type())
				{
					case trent::type::boolean:
						os.print(unsafe_bool_const() ? "true" : "false");
						return 0;

					case trent::type::numer:
						os.print(unsafe_numer_const());
						return 0;

					case trent::type::integer:
						os.print(unsafe_integer_const());
						return 0;

					case trent::type::string:
						os.putchar('"');
						gxx::print_to(os, unsafe_string_const());
						os.putchar('"');
						return 0;
					case trent::type::list:
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
						return 0;
					case trent::type::nil:
						os.print("nil");
						return 0;
				}

				PANIC_TRACED();
				return 0;
			}*/
	};

	using trent = trent_basic<std::allocator>;


	template <template <class Allocator> class TAlloc>
	trent_basic<TAlloc>::~trent_basic()
	{
		invalidate();
	}

	template <template <class Allocator> class TAlloc>
	void trent_basic<TAlloc>::invalidate()
	{
		switch (m_type)
		{
			case trent::type::string:
				igris::destructor(&m_str);
				return;
			case trent::type::list:
				igris::destructor(&m_arr);
				return;
			case trent::type::dict:
				igris::destructor(&m_dict);
				return;
			case type::nil:
			case type::integer:
			case type::numer:
			case type::boolean:
				return;
			default:
				BUG();
				return;
		}
		m_type = trent::type::nil;
	}

}

#endif

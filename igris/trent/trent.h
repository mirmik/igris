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
#include <igris/buffer.h>

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
			//	integer,
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
			bool m_bool;
			numer_type m_num;
			//	integer_type m_int;
			list_type m_arr;
			dict_type m_dict;
			string_type m_str;
		};

	public:
		~trent_basic();
		trent_basic() : m_type(type::nil) {}
		trent_basic(const trent_basic& other);

		void invalidate();

		/*inline trent_basic(const type& t) { init(t); }
		inline trent_basic(const std::string& str) { init(str); }
		inline trent_basic(const char* str) { init(str); }
		inline trent_basic(const float& i) { init(i); }
		inline trent_basic(const double& i) { init(i); }
		inline trent_basic(const long double& i) { init(i); }
		inline trent_basic(const signed char& i) { init(i); }
		inline trent_basic(const signed short& i) { init(i); }
		inline trent_basic(const signed int& i) { init(i); }
		inline trent_basic(const signed long& i) { init(i); }
		inline trent_basic(const signed long long& i) { init(i); }
		inline trent_basic(const unsigned char& i) { init(i); }
		inline trent_basic(const unsigned short& i) { init(i); }
		inline trent_basic(const unsigned int& i) { init(i); }
		inline trent_basic(const unsigned long& i) { init(i); }
		inline trent_basic(const unsigned long long& i) { init(i); }
		inline trent_basic(const bool& i) { init(i); }*/

		template <class T>
		trent_basic(const T& obj)
		{
			init(obj);
		}

		void throw_wrong_type_exception()
		{
			throw std::exception();
		}

		void throw_wrong_list_size_exception()
		{
			throw std::exception();
		}

	public:
		void init_sint(const int64_t& i) { m_type = type::numer; m_num = i; }
		void init_uint(const uint64_t& i) { m_type = type::numer; m_num = i; }
		void init_flt(const long double& i) { m_type = type::numer; m_num = i; }
		void init_str(const char* data, size_t size);

		void init(type t);
		void init(const trent_basic& oth);

		void init(const char* ptr) { init_str(ptr, strlen(ptr)); }
		void init(const std::string& str) { init_str(str.data(), str.size()); }
		void init(const igris::buffer& str) { init_str(str.data(), str.size()); }

		void init(const int8_t& i) { init_sint(i); }
		void init(const int16_t& i) { init_sint(i); }
		void init(const int32_t& i) { init_sint(i); }
		void init(const int64_t& i) { init_sint(i); }

		void init(const uint8_t& i) { init_uint(i); }
		void init(const uint16_t& i) { init_uint(i); }
		void init(const uint32_t& i) { init_uint(i); }
		void init(const uint64_t& i) { init_uint(i); }

		void init(const float& i) { init_flt(i); }
		void init(const double& i) { init_flt(i); }
		void init(const long double& i) { init_flt(i); }

		void init(const bool& i) { m_type = type::boolean; m_bool = i; }

		template <typename T>
		void reset(const T& obj)
		{
			invalidate();
			init(obj);
		}

		/*	void init_list(size_t reserve);
		*/
	public:
		// Поведение - переписать значение, если тип не совпадает.
		const trent_basic& operator[](int i) const;
		const trent_basic& operator[](const char* key) const;
		const trent_basic& operator[](const std::string& key) const;
		const trent_basic& operator[](const igris::buffer& key) const;
		//const trent_basic& operator[](const trent_path& path) const;

		trent_basic& operator[](int i);
		trent_basic& operator[](const char* key);
		trent_basic& operator[](const std::string& key);
		trent_basic& operator[](const igris::buffer& key);
		//trent_basic& operator[](const trent_path& path);

		// Поведение - выдать исключение если тип не совпадает.
		const trent_basic& at(int i) const;
		const trent_basic& at(const char* key) const;
		const trent_basic& at(const std::string& key) const;
		const trent_basic& at(const igris::buffer& key) const;
		//const trent_basic& at(const trent_path& path) const;

		trent_basic& at(int i);
		trent_basic& at(const char* key);
		trent_basic& at(const std::string& key);
		trent_basic& at(const igris::buffer& key);
		//trent_basic& at(const trent_path& path);

		/*			bool have(const std::string& key) const;

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
					const igris::buffer as_buffer() const;
					string_type& as_string_default(string_type& str);
					result<string_type&> as_string_critical();
					result<const string_type&> as_string_critical() const;

					integer_type& unsafe_integer_const() { return m_int; }
					numer_type& unsafe_numer_const() { return m_num; }
					string_type& unsafe_string_const() { return m_str; }
					list_type& unsafe_list_const() { return m_arr; }
					dict_type& unsafe_dict_const() { return m_dict; }
				*/
//			const integer_type& unsafe_integer_const() const { return m_int; }
		const numer_type& unsafe_numer_const() const { return m_num; }
		const string_type& unsafe_string_const() const { return m_str; }
		const list_type& unsafe_list_const() const { return m_arr; }
		const dict_type& unsafe_dict_const() const { return m_dict; }
		const bool& unsafe_bool_const() const { return m_bool; }

		trent_basic::type get_type() const { return m_type; }
		const char * typestr() const;

		bool is_nil() const 		{ return m_type == type::nil; }
		bool is_bool() const 		{ return m_type == type::boolean; }
		bool is_numer() const 		{ return m_type == type::numer; }
		//bool is_integer() const     { return m_type == type::integer; }
		bool is_list() const		{ return m_type == type::list; }
		bool is_dict() const        { return m_type == type::dict; }
		bool is_string() const 		{ return m_type == type::string; }

		//strlst check_dict(strlst lst, check_type ct);
		//std::pair<strlst,strlst> check_dict_symmetric(strlst lst);

		template <class O>
		ssize_t print_to(O& os) const;

	public:
		template <class T>
		trent_basic& operator= (const T& arg)
		{
			reset(arg);
		}

		/*			ssize_t size();

					bool contains(igris::buffer buf);
		*/

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

			//case type::integer:
			case type::numer:
			case type::boolean:
				return;

			default:
				BUG();
				return;
		}

		m_type = trent::type::nil;
	}

	template <template <class Allocator> class TAlloc>
	void trent_basic<TAlloc>::init_str(const char* data, size_t size)
	{
		m_type = trent_basic::type::string;
		igris::constructor(&m_str, data, size);
	}

	/*emplate <template <class Allocator> class TAlloc>
	void trent_basic<TAlloc>::init(const trent_basic::numer_type& n)
	{
		m_type = trent_basic::type::numer;
		m_num = n;
	}*/

	/*template <template <class Allocator> class TAlloc>
	void trent_basic<TAlloc>::init(
	    const igris::numer_ctrcollect<numer_type>& n)
	{
		m_type = trent_basic::type::numer;
		m_num = n;
	}

	template <template <class Allocator> class TAlloc>
	void trent_basic<TAlloc>::init(const bool_ctrcollect<bool>& n)
	{
		m_type = trent_basic::type::boolean;
		m_bool = n;
	}*/

	template <template <class Allocator> class TAlloc>
	template <class O>
	ssize_t trent_basic<TAlloc>::print_to(O& os) const
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

			//case trent::type::integer:
			//	os.print(unsafe_integer_const());
			//	return 0;

			case trent::type::string:
				os.putchar('"');
				os.print(unsafe_string_const());
				os.putchar('"');
				return 0;

			case trent::type::list:
				os.putchar('[');

				for (auto& v : unsafe_list_const())
				{
					if (sep) os.putchar(',');

					v.print_to(os);
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
					os.print(p.first);
					os.putchar('"');
					os.putchar(':');
					p.second.print_to(os);
					sep = true;
				}

				os.putchar('}');
				return 0;

			case trent::type::nil:
				os.print("nil");
				return 0;
		}

		BUG();
		return 0;
	}


	template <template <class Allocator> class TAlloc>
	const char * trent_basic<TAlloc>::typestr() const
	{
		switch (m_type)
		{
			case trent_basic::type::string: 	return "string";

			case trent_basic::type::list: 		return "list";

			case type::dict:       return "dict";

			case trent_basic::type::numer: 		return "numer";

			//case trent_basic::type::integer: 		return "integer";
			case trent_basic::type::boolean:    return "bool";

			case trent_basic::type::nil:        return "nil";

			default: BUG();
		}

		return "";
	}




	template <template <class Allocator> class TAlloc>
	trent_basic<TAlloc>& trent_basic<TAlloc>::operator[](int i)
	{
		if (m_type != type::list)
			init(type::list);

		if (m_arr.size() <= (unsigned int)i) m_arr.resize(i + 1);

		return m_arr[i];
	}

	template <template <class Allocator> class TAlloc>
	const trent_basic<TAlloc>& trent_basic<TAlloc>::operator[](int key) const
	{
		if (m_type != trent_basic::type::list)
			init(type::list);

		return m_arr.at(key);
	}

	template <template <class Allocator> class TAlloc>
	trent_basic<TAlloc>& trent_basic<TAlloc>::operator[](const char* key)
	{
		if (m_type != type::dict)
			init(type::dict);

		return m_dict[key];
	}

	template <template <class Allocator> class TAlloc>
	trent_basic<TAlloc>& trent_basic<TAlloc>::operator[](const std::string& key)
	{
		if (m_type != type::dict) init(type::dict);

		return m_dict[key];
	}

	template <template <class Allocator> class TAlloc>
	const trent_basic<TAlloc>& trent_basic<TAlloc>::operator[](const std::string& key) const
	{
		if (m_type != type::dict)
			init(type::dict);

		return m_dict.at(key);
	}

	template <template <class Allocator> class TAlloc>
	trent_basic<TAlloc>& trent_basic<TAlloc>::operator[](const igris::buffer& key)
	{
		if (m_type != type::dict)
			init(type::dict);

		return m_dict[std::string(key.data(), key.size())];
	}

	/*template <template <class Allocator> class TAlloc>
	const trent_basic<TAlloc>& trent_basic::operator[] (const igris::trent_basic_path& path) const
	{
		const igris::trent_basic* tr = this;

		for (const auto& p : path)
		{
			if (p.is_string)
			{
				tr = &tr->operator[](p.str);
			}
			else
			{
				tr = &tr->operator[](p.i32);
			}
		}

		return *tr;
	}*/

	/*template <template <class Allocator> class TAlloc>
	trent_basic& trent_basic::operator[] (const igris::trent_basic_path& path)
	{
		igris::trent_basic* tr = this;

		for (auto& p : path)
		{
			if (p.is_string)
			{
				tr = &tr->operator[](p.str);
			}
			else
			{
				tr = &tr->operator[](p.i32);
			}
		}

		return *tr;
	}*/

	template <template <class Allocator> class TAlloc>
	const trent_basic<TAlloc>& trent_basic<TAlloc>::at(int i) const
	{
		if (m_type != trent_basic::type::list)
			throw_wrong_type_exception();

		if (m_arr.size() <= (unsigned int)i)
			throw_wrong_list_size_exception();

		return m_arr[i];
	}

	template <template <class Allocator> class TAlloc>
	const trent_basic<TAlloc>& trent_basic<TAlloc>::at(const char* key) const
	{
		if (m_type != type::dict)
			throw_wrong_type_exception();

		return m_dict[key];
	}

	template <template <class Allocator> class TAlloc>
	const trent_basic<TAlloc>& trent_basic<TAlloc>::at(const std::string& key) const
	{
		if (m_type != type::dict)
			throw_wrong_type_exception();

		return m_dict.at(key);
	}

	template <template <class Allocator> class TAlloc>
	const trent_basic<TAlloc>& trent_basic<TAlloc>::at(const igris::buffer& key) const
	{
		if (m_type != type::dict)
			throw_wrong_type_exception();

		return m_dict.at(std::string(key.data(), key.size()));
	}

	template <template <class Allocator> class TAlloc>
	void trent_basic<TAlloc>::init(trent_basic::type t)
	{
		m_type = t;

		switch (m_type)
		{
			case trent_basic::type::string:
				igris::constructor(&m_str);
				return;

			case trent_basic::type::list:
				igris::constructor(&m_arr);
				return;

			case trent_basic::type::dict:
				igris::constructor(&m_dict);
				return;

			//case trent_basic::type::integer:
			case trent_basic::type::numer:
			case trent_basic::type::nil:
				return;

			default:
				BUG();
		}
	}

	template <template <class Allocator> class TAlloc>
	trent_basic<TAlloc>::trent_basic(const trent_basic<TAlloc>& other)
	{
		m_type = other.m_type;

		switch (m_type)
		{
			case trent_basic::type::string:
				igris::constructor(&m_str, other.m_str);
				return;

			case trent_basic::type::list:
				igris::constructor(&m_arr, other.m_arr);
				return;

			case trent_basic::type::dict:
				igris::constructor(&m_dict, other.m_dict);
				return;

			case trent_basic::type::numer:
				m_num = other.m_num;
				return;

			case trent_basic::type::boolean:
				//case trent_basic::type::integer:
				//    m_int = other.m_int;
				return;

			case trent_basic::type::nil:
				return;

			default:
				BUG();
		}
	}

}

#endif

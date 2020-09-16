#ifndef GXX_DATATREE_H
#define GXX_DATATREE_H

#include <cstdint>
#include <cassert>
#include <utility>

#include <string>
#include <vector>
#include <map>

#include <igris/util/types_extension.h>
#include <igris/util/ctrdtr.h>
#include <igris/util/bug.h>
#include <igris/buffer.h>
#include <igris/result.h>

#include <igris/trent/trent_path.h>

using namespace igris::result_type;

namespace igris
{
	enum class trent_type : uint8_t
	{
		string,
		list,
		dict,
		numer,
		boolean,
		nil,
	};
	const char * typestr(trent_type type);

	class trent_path;

	template <template <class Allocator> class TAlloc>
	class trent_basic
	{
	public:
		//SPECS
		using type = trent_type;
		using value_type = std::pair<std::string, trent_basic>;

		using valloc_t = std::allocator<trent_basic>;
		using malloc_t = std::allocator<std::pair<const std::string, trent_basic>>;

		class wrong_path : public std::exception
		{
			trent_path path;
			std::string str;

		public:
			wrong_path(const igris::trent_path& path) :
				path(path)
			{
				str = std::string("trent:wrong_path: ") + 
					path.to_string();
			}

			const char * what() const noexcept override
			{
				return str.c_str();
			}
		};

		class wrong_type : public std::exception
		{
			trent_path path;
			type t;
			std::string str;

		public:
			wrong_type(const trent_path& path, type t, type rt) :
				path(path), t(t)
			{
				str =
				    std::string("trent:wrong_type:") +
				    std::string(" path:") + path.to_string() +
				    std::string(" request:") + igris::typestr(t) +
				    std::string(" realtype:") + igris::typestr(rt);
			}

			const char * what() const noexcept override
			{
				return str.c_str();
			}
		};

		class wrong_index : public std::exception
		{
			trent_path path;
			type t;

		public:
			wrong_index(const trent_path& path, type t) :
				path(path), t(t)
			{}

			const char * what() const noexcept override
			{
				std::string str =
				    std::string("trent:wrong_index: path: ") + path.to_string() +
				    std::string(" index: ") + igris::typestr(t);
				return str.c_str();
			}
		};


		using numer_type = long double;
		using integer_type = int64_t;
		using list_type = std::vector<trent_basic, valloc_t>;
		using dict_type = std::map<std::string, trent_basic, std::less<std::string>, malloc_t>;
		using string_type = std::string;

	protected:
		type m_type = type::nil;
		union
		{
			bool m_bool;
			numer_type m_num;
			list_type m_arr;
			dict_type m_dict;
			string_type m_str;
		};

	public:
		const char * typestr() { return igris::typestr(m_type); }

		~trent_basic();
		trent_basic() : m_type(type::nil) {}
		trent_basic(const trent_basic& other);
		trent_basic(trent_basic&& other);

		void invalidate();

		template <class T>
		trent_basic(const T& obj)
		{
			init(obj);
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
		//const trent_basic& operator[](int i) const;
		//const trent_basic& operator[](const char* key) const;
		//const trent_basic& operator[](const std::string& key) const;
		//const trent_basic& operator[](const igris::buffer& key) const;
		//const trent_basic& operator[](const trent_path& path) const;

		//trent_basic& operator[](int i);
		//trent_basic& operator[](const char* key);
		//trent_basic& operator[](const std::string& key);
		//trent_basic& operator[](const igris::buffer& key);
		//trent_basic& operator[](const trent_path& path);

		// Поведение - выдать исключение если тип не совпадает.
		/*		const trent_basic& at(int i) const;
				const trent_basic& at(const char* key) const;
				const trent_basic& at(const std::string& key) const;
				const trent_basic& at(const igris::buffer& key) const;
				const trent_basic& at(const trent_path& path) const;

				trent_basic& at(int i);
				trent_basic& at(const char* key);
				trent_basic& at(const std::string& key);
				trent_basic& at(const igris::buffer& key);
				trent_basic& at(const trent_path& path);*/










		trent_basic& operator[](int i)
		{
			if (m_type != type::list) init(type::list);
			if (m_arr.size() <= (unsigned int)i) m_arr.resize(i + 1);
			return m_arr[i];
		}

		const trent_basic& operator[](int key) const
		{
			if (m_type != type::list) BUG_ON("wrong trent type");
			return m_arr.at(key);
		}

		trent_basic& operator[](const char* key)
		{
			if (m_type != type::dict) init(type::dict);
			return m_dict[key];
		}

		trent_basic& operator[](const std::string& key)
		{
			if (m_type != type::dict) init(type::dict);
			return m_dict[key];
		}

		const trent_basic& operator[](const std::string& key) const
		{
			if (m_type != type::dict) BUG_ON("wrong trent type");
			return m_dict.at(key);
		}

		trent_basic& operator[](const igris::buffer& key)
		{
			if (m_type != type::dict) init(type::dict);
			return m_dict[std::string(key.data(), key.size())];
		}

		const trent_basic& operator[] (const trent_path& path) const
		{
			const trent_basic* tr = this;
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
		}

		trent_basic& operator[] (const trent_path& path)
		{
			trent_basic* tr = this;
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
		}

		const trent_basic& at(int i) const
		{
			if (m_type != type::list) BUG_ON("wrong trent type");
			if (m_arr.size() <= (unsigned int)i) BUG_ON("wrong trent list size");
			return m_arr[i];
		}


		const trent_basic& operator[](const char* key) const
		{
			if (m_type != type::dict) BUG_ON("wrong trent type");
			return m_dict.at(key);
		}

		/*const trent& trent::at(const char* key) const {
		gxx::println(2);
		if (m_type != type::dict) init(type::dict);
		        return m_dict[key];
		}*/

		const trent_basic& at(const std::string& key) const
		{
			if (m_type != type::dict) BUG_ON("wrong trent type");
			return m_dict.at(key);
		}

		const trent_basic& at(const igris::buffer& key) const
		{
			if (m_type != type::dict) BUG_ON("wrong trent type");
			return m_dict.at(std::string(key.data(), key.size()));
		}

		const trent_basic * _get(const std::string& str) const
		{
			if (is_dict())
			{
				auto it = m_dict.find(str);
				if (it == m_dict.end())
					return nullptr;
				return &it->second;
			}

			return nullptr;
		}

		const trent_basic * _get(const char * str) const
		{
			return get(std::string(str));
		}

		const trent_basic * _get(int index) const
		{
			if (is_list())
				return &m_arr[index];

			return nullptr;
		}

		const trent_basic * get(const trent_path& path) const
		{
			const trent_basic* tr = this;
			for (const auto& p : path)
			{
				if (p.is_string)
				{
					if (!tr->is_dict())
						return nullptr;
					tr = tr->_get(p.str);
				}
				else
				{
					tr = tr->_get(p.i32);
				}

				if (tr == nullptr)
					return nullptr;
			}
			return tr;
		}

		const trent_basic & get_except(const trent_path & path) const
		{
			const trent_basic * tr = get(path);
			if (tr == nullptr)
			{
				throw wrong_path(path);
			}
			return *tr;
		}

		numer_type get_as_numer_ex(const trent_path& path) const
		{
			const trent_basic & tr = get_except(path);

			if (tr.m_type != trent_type::numer)
			{
				throw wrong_type(path, trent_type::numer, tr.m_type);
			}

			return tr.m_num;
		}

		const string_type& get_as_string_ex(const trent_path& path) const
		{
			const trent_basic & tr = get_except(path);

			if (tr.m_type != trent_type::string)
			{
				throw wrong_type(path, trent_type::string, tr.m_type);
			}

			return tr.m_str;
		}

		const bool get_as_boolean_ex(const trent_path& path) const
		{
			const trent_basic & tr = get_except(path);

			if (tr.m_type != trent_type::boolean)
			{
				throw wrong_type(path, trent_type::string, tr.m_type);
			}

			return tr.m_bool;
		}

		numer_type get_as_numer_def(const trent_path& path, numer_type def) const
		{
			const trent_basic * tr = get(path);
			if (tr == nullptr || tr->m_type != trent_type::numer)
				return def;
			return tr->m_num;
		}

		const string_type& get_as_string_def(const trent_path& path, const std::string& def) const
		{
			const trent_basic * tr = get(path);
			if (tr == nullptr || tr->m_type != trent_type::string)
				return def;
			return tr->m_str;
		}

		const bool get_as_boolean_def(const trent_path& path, bool def) const
		{
			const trent_basic * tr = get(path);
			if (tr == nullptr || tr->m_type != trent_type::boolean)
				return def;
			return tr->m_bool;
		}

		bool have(const std::string& key) const
		{
			if (m_type != type::dict) BUG_ON("wrong trent type");
			return m_dict.count(key) != 0;
		}

		string_type& as_string() { if (m_type != type::string) init(type::string); return m_str; }
		const string_type& as_string() const { if (m_type != type::string) BUG(); return m_str; }
		result<string_type&> as_string_critical() { if (!is_string()) return error("is't string"); return m_str; }
		result<const string_type&> as_string_critical() const { if (!is_string()) return error("is't string"); return m_str; }
		string_type& as_string_except();
		const string_type& as_string_except() const;
		const string_type& as_string_default(const string_type& def) const { if (!is_string()) return def; return m_str; }

		dict_type& as_dict();
		const dict_type& as_dict() const;
		result<dict_type&> as_dict_critical();
		result<const dict_type&> as_dict_critical() const;
		dict_type& as_dict_except();
		const dict_type& as_dict_except() const;

		list_type& as_list() { if (m_type != type::list) init(type::list); return m_arr; }
		const list_type& as_list() const { if (m_type != type::list) BUG(); return m_arr; }
		result<list_type&> as_list_critical() { if (!is_list()) return error("is't list"); return m_arr; }
		result<const list_type&> as_list_critical() const { if (!is_list()) return error("is't list"); return m_arr; }
		list_type& as_list_except();
		const list_type& as_list_except() const;

		numer_type as_numer() { if (m_type != type::numer) init(type::numer); return m_num; }
		numer_type as_numer() const { if (m_type != type::numer) BUG(); return m_num; }
		numer_type as_numer_default(numer_type def) const { if (!is_numer()) return def; return m_num; }
		result<numer_type> as_numer_critical() const { if (!is_numer()) return error("is't numer"); return m_num; }
		numer_type as_numer_except() const;

		int64_t as_integer() const { return as_numer(); }
		int64_t as_integer_default(int64_t def) const { return as_numer_default(def); }

		//TODO добавить проверку на целое.
		result<int64_t> as_integer_critical() const { if (!is_numer()) return error("is't numer"); return m_num; }
		int64_t as_integer_except() const { if (!is_numer()) throw std::runtime_error("is't numer"); return m_num; }

		bool as_bool() const { return m_bool; }
		bool as_bool_default(bool def) const { if (!is_bool()) return def; return m_bool; }

		//TODO добавить проверку на целое.
		result<bool> as_bool_critical() const { if (!is_bool()) return error("is't bool"); return m_bool; }
		bool as_bool_except() const { if (!is_bool()) throw std::runtime_error("is't bool"); return m_bool; }


		const igris::buffer as_buffer() const;

		//integer_type& unsafe_integer_const() { return m_int; }
		numer_type& unsafe_numer_const() { return m_num; }
		string_type& unsafe_string_const() { return m_str; }
		list_type& unsafe_list_const() { return m_arr; }
		dict_type& unsafe_dict_const() { return m_dict; }

//			const integer_type& unsafe_integer_const() const { return m_int; }
		const numer_type& unsafe_numer_const() const { return m_num; }
		const string_type& unsafe_string_const() const { return m_str; }
		const list_type& unsafe_list_const() const { return m_arr; }
		const dict_type& unsafe_dict_const() const { return m_dict; }
		const bool& unsafe_bool_const() const { return m_bool; }

		trent_basic::type get_type() const { return m_type; }

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
		//trent_basic& operator=(const trent_basic& other);
		//trent_basic& operator=(trent_basic&& other);

		trent_basic& operator= (const trent_basic& other)
		{
			invalidate();
			m_type = other.m_type;
			switch (m_type)
			{
				case type::string:
					igris::constructor(&m_str, other.m_str);
					return *this;
				case type::list:
					igris::constructor(&m_arr, other.m_arr);
					return *this;
				case type::dict:
					igris::constructor(&m_dict, other.m_dict);
					return *this;
				case type::numer:
					m_num = other.m_num;
					return *this;
				//case type::integer:
				//    m_int = other.m_int;
				//	return *this;
				case type::nil:
					return *this;
				default:
					BUG();
			}
			return *this;
		}

		trent_basic& operator= (trent_basic&& other)
		{
			invalidate();
			m_type = other.m_type;
			switch (m_type)
			{
				case type::string:
					igris::move_constructor(&m_str, std::move(other.m_str));
					return *this;
				case type::list:
					igris::move_constructor(&m_arr, std::move(other.m_arr));
					return *this;
				case type::dict:
					igris::move_constructor(&m_dict, std::move(other.m_dict));
					return *this;
				case type::numer:
					m_num = other.m_num;
					return *this;
				//case type::integer:
				//    m_int = other.m_int;
				//	return *this;
				case type::nil:
					return *this;
				default:
					BUG();
			}

			other.reset(type::nil);
			return *this;
		}

		template <class T>
		trent_basic& operator= (const T& arg)
		{
			reset(arg);
			return *this;
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
			case type::string:
				igris::destructor(&m_str);
				return;

			case type::list:
				igris::destructor(&m_arr);
				return;

			case type::dict:
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

		m_type = type::nil;
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
			case type::boolean:
				os.print(unsafe_bool_const() ? "true" : "false");
				return 0;

			case type::numer:
				os.print(unsafe_numer_const());
				return 0;

			//case type::integer:
			//	os.print(unsafe_integer_const());
			//	return 0;

			case type::string:
				os.putchar('"');
				os.print(unsafe_string_const());
				os.putchar('"');
				return 0;

			case type::list:
				os.putchar('[');

				for (auto& v : unsafe_list_const())
				{
					if (sep) os.putchar(',');

					v.print_to(os);
					sep = true;
				}

				os.putchar(']');
				return 0;

			case type::dict:
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

			case type::nil:
				os.print("nil");
				return 0;
		}

		BUG();
		return 0;
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
				m_bool = other.m_bool;
				return;

			case trent_basic::type::nil:
				return;

			default:
				BUG();
		}
	}

	template <template <class Allocator> class TAlloc>
	trent_basic<TAlloc>::trent_basic(trent_basic<TAlloc>&& other)
	{
		m_type = other.m_type;

		switch (m_type)
		{
			case trent_basic::type::string:
				igris::move_constructor(&m_str, std::move(other.m_str));
				return;

			case trent_basic::type::list:
				igris::move_constructor(&m_arr, std::move(other.m_arr));
				return;

			case trent_basic::type::dict:
				igris::move_constructor(&m_dict, std::move(other.m_dict));
				return;

			case trent_basic::type::numer:
				m_num = other.m_num;
				return;

			case trent_basic::type::boolean:
				m_bool = other.m_bool;
				return;

			case trent_basic::type::nil:
				return;

			default:
				BUG();
		}

		other.invalidate();
	}


	template <template <class Allocator> class TAlloc>
	std::vector<trent_basic<TAlloc>>& trent_basic<TAlloc>::as_list_except()
	{
		if (!is_list())
			throw std::runtime_error("isn't list");

		return m_arr;
	}

	template <template <class Allocator> class TAlloc>
	const std::vector<trent_basic<TAlloc>>& trent_basic<TAlloc>::as_list_except() const
	{
		if (!is_list())
			throw std::runtime_error("isn't list");

		return m_arr;
	}




	template <template <class Allocator> class TAlloc>
	typename trent_basic<TAlloc>::dict_type& trent_basic<TAlloc>::as_dict()
	{
		if (m_type != type::dict)
			init(type::dict);

		return m_dict;
	}

	template <template <class Allocator> class TAlloc>
	typename trent_basic<TAlloc>::dict_type& trent_basic<TAlloc>::as_dict_except()
	{
		if (!is_dict())
			throw std::runtime_error("isn't dict");

		return m_dict;
	}

	template <template <class Allocator> class TAlloc>
	const typename trent_basic<TAlloc>::dict_type& trent_basic<TAlloc>::as_dict_except() const
	{
		if (!is_dict())
			throw std::runtime_error("isn't dict");

		return m_dict;
	}



	/*template <template <class Allocator> class TAlloc>
	std::string& trent_basic<TAlloc>::as_string()
	{
		if (m_type != type::string)
			init(type::string);

		return m_str;
	}*/

	template <template <class Allocator> class TAlloc>
	std::string& trent_basic<TAlloc>::as_string_except()
	{
		if (!is_string())
			throw std::runtime_error("isn't string");

		return m_str;
	}

	template <template <class Allocator> class TAlloc>
	const std::string& trent_basic<TAlloc>::as_string_except() const
	{
		if (!is_string())
			throw std::runtime_error("isn't string");

		return m_str;
	}
}

#endif

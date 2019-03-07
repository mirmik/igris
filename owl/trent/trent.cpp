#include <nos/trent/trent.h>
#include <algorithm>

#include <nos/print.h>

using namespace nos::result_type;

namespace nos
{

	trent::~trent()
	{
		invalidate();
	}

	trent::trent() {}

	trent::trent(const trent& other)
	{
		m_type = other.m_type;
		switch (m_type)
		{
			case trent::type::string:
				new (&m_str) std::string(other.m_str);
				return;
			case trent::type::list:
				new (&m_arr) list_type(other.m_arr);
				return;
			case trent::type::dict:
				new (&m_dict) dict_type(other.m_dict);
				return;
			case trent::type::numer:
				m_num = other.m_num;
				return;
			case trent::type::boolean:
			case trent::type::integer:
				m_int = other.m_int;
				return;
			case trent::type::nil:
				return;
			default:
				abort();
		}
	}

	//trent::trent(const std::string& str) { init(str); }
	//trent::trent(const char* str) { init(str); }
	//trent::trent(const trent::type& t) { init(t); }
	//trent::trent(const float& i) { init(i); }
	//trent::trent(const double& i) { init(i); }
	//trent::trent(const long double& i) { init(i); }
	//trent::trent(const signed char& i) { init(i); }
	//trent::trent(const signed short& i) { init(i); }
	//trent::trent(const signed int& i) { init(i); }
	//trent::trent(const signed long& i) { init(i); }
	//trent::trent(const signed long long& i) { init(i); }
	//trent::trent(const unsigned char& i) { init(i); }
	//trent::trent(const unsigned short& i) { init(i); }
	//trent::trent(const unsigned int& i) { init(i); }
	//trent::trent(const unsigned long& i) { init(i); }
	//trent::trent(const unsigned long long& i) { init(i); }

	void trent::init(trent::type t)
	{
		m_type = t;
		switch (m_type)
		{
			case trent::type::string:
				new (&m_str) std::string();
				return;
			case trent::type::list:
				new (&m_arr) list_type();
				return;
			case trent::type::dict:
				new (&m_dict) dict_type();
				return;
			case trent::type::integer:
			case trent::type::numer:
			case trent::type::nil:
				return;
			default:
				abort();
				//PANIC_TRACED();
		}
	}

	void trent::init(const std::string& str)
	{
		m_type = trent::type::string;
		new (&m_str) std::string(str);

	}

	void trent::init(const char* str)
	{
		m_type = trent::type::string;
		new (&m_str) std::string(str);
	}

	//void trent::init(owl::buffer buf) {
	//	init(std::string(buf.data(), buf.size()));
	//}

	void trent::init(const float& n) { m_type = trent::type::numer; m_num = n; }
	void trent::init(const double& n) { m_type = trent::type::numer; m_num = n; }
	void trent::init(const long double& n) { m_type = trent::type::numer; m_num = n; }

	void trent::init(const signed char& n) { m_type = trent::type::integer; m_int = n; }
	void trent::init(const signed short& n) { m_type = trent::type::integer; m_int = n; }
	void trent::init(const signed int& n) { m_type = trent::type::integer; m_int = n; }
	void trent::init(const signed long& n) { m_type = trent::type::integer; m_int = n; }
	void trent::init(const signed long long& n) { m_type = trent::type::integer; m_int = n; }
	void trent::init(const unsigned char& n) { m_type = trent::type::integer; m_int = n; }
	void trent::init(const unsigned short& n) { m_type = trent::type::integer; m_int = n; }
	void trent::init(const unsigned int& n) { m_type = trent::type::integer; m_int = n; }
	void trent::init(const unsigned long& n) { m_type = trent::type::integer; m_int = n; }
	void trent::init(const unsigned long long& n) { m_type = trent::type::integer; m_int = n; }
	void trent::init(const bool& n) { m_type = trent::type::boolean; m_int = n; }

	void trent::invalidate()
	{
		switch (m_type)
		{
			case trent::type::string:
				m_str.~basic_string();
				return;
			case trent::type::list:
				m_arr.~list_type();
				return;
			case trent::type::dict:
				m_dict.~dict_type();
				//dict_type::~dict_type(&m_dict);
				return;
			case trent::type::nil:
			case trent::type::integer:
			case trent::type::numer:
			case trent::type::boolean:
				return;
			default:
				//unreacheble code
				abort();
				return;
		}
		m_type = trent::type::nil;
	}

	trent& trent::operator[](int i)
	{
		if (m_type != trent::type::list) init(trent::type::list);
		if (m_arr.size() <= (unsigned int)i) m_arr.resize(i + 1);
		return m_arr[i];
	}

	const trent& trent::operator[](int key) const
	{
		if (m_type != trent::type::list) BUG_ON("wrong trent type");
		return m_arr.at(key);
	}

	trent& trent::operator[](const char* key)
	{
		if (m_type != trent::type::dict) init(trent::type::dict);
		return m_dict[key];
	}

	trent& trent::operator[](const std::string& key)
	{
		if (m_type != trent::type::dict) init(trent::type::dict);
		return m_dict[key];
	}

	const trent& trent::operator[](const std::string& key) const
	{
		if (m_type != trent::type::dict) BUG_ON("wrong trent type");
		return m_dict.at(key);
	}

	/*trent& trent::operator[](const nos::buffer& key)
	{
		if (m_type != trent::type::dict) init(trent::type::dict);
		return m_dict[std::string(key.data(), key.size())];
	}*/

	/*		const trent& trent::operator[] (const owl::trent_path& path) const {
				const owl::trent* tr = this;
				for (const auto& p : path) {
					if (p.is_string) {
						tr = &tr->operator[](p.str);
					}
					else {
						tr = &tr->operator[](p.i32);
					}
				}
				return *tr;
			}
	*/
	/*trent& trent::operator[] (const owl::trent_path& path)
	{
		owl::trent* tr = this;
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
	/*
			const trent& trent::at(int i) const {
				if (m_type != trent::type::list) owl::panic("wrong trent type");
		        if(m_arr.size() <= (unsigned int)i) owl::panic("wrong trent list size");
				return m_arr[i];
			}

	*/
	const trent& trent::operator[](const char* key) const
	{
		if (m_type != trent::type::dict) BUG_ON("wrong trent type");
		return m_dict.at(key);
	}

	/*const trent& trent::at(const char* key) const {
	owl::println(2);
	if (m_type != trent::type::dict) init(trent::type::dict);
	        return m_dict[key];
	}*/

	const trent& trent::at(const std::string& key) const
	{
		if (m_type != trent::type::dict) BUG_ON("wrong trent type");
		return m_dict.at(key);
	}

	/*const trent& trent::at(const nos::buffer& key) const {
	    if (m_type != trent::type::dict) BUG_ON("wrong trent type");
		return m_dict.at(std::string(key.data(), key.size()));
	}*/

	bool trent::have(const std::string& str) const
	{
		if (m_type != trent::type::dict) BUG_ON("wrong trent type");
		return m_dict.count(str) != 0;
	}

	std::map<std::string, trent>& trent::as_dict()
	{
		if (m_type != trent::type::dict) init(trent::type::dict);
		return m_dict;
	}

	const std::map<std::string, trent>& trent::as_dict() const
	{
		if (m_type != trent::type::dict) BUG_ON("wrong_trent_type");
		return m_dict;
	}

	std::vector<trent>& trent::as_list()
	{
		if (m_type != trent::type::list) init(trent::type::list);
		return m_arr;
	}

	const std::vector<trent>& trent::as_list() const
	{
		if (m_type != trent::type::list) BUG_ON("wrong_trent_type");
		return m_arr;
	}

	result<std::vector<trent>&> trent::as_list_critical()
	{
		if (!is_list()) return error("is't list");
		return m_arr;
	}
	result<const std::vector<trent>&> trent::as_list_critical() const
	{
		if (!is_list()) return error("is't list");
		return m_arr;
	}

	std::string& trent::as_string()
	{
		if (m_type != trent::type::string) init(trent::type::string);
		return m_str;
	}
	const std::string& trent::as_string() const
	{
		if (m_type != trent::type::string) BUG_ON("wrong_trent_type");
		return m_str;
	}

	const nos::buffer trent::as_buffer() const
	{
		if (m_type == trent::type::string) return nos::buffer(m_str.data(), m_str.size());
		return nos::buffer();
	}

	trent::numer_type trent::as_numer() const
	{
		if (m_type == trent::type::numer) return m_num;
		if (m_type == trent::type::integer) return m_int;
		return 0;
	}

	trent::integer_type trent::as_integer() const
	{
		if (m_type == trent::type::integer) return m_int;
		return 0;
	}
	/*
			        result<trent::integer_type>  trent::as_integer_critical() const {
			                if (!is_integer()) return error("is't integer");
			                return m_int;
			        }

				result<std::string&> trent::as_string_critical() {
					if (!is_string()) return error("is't string");
					return m_str;
				}

				result<const std::string&> trent::as_string_critical() const {
					if (!is_string()) return error("is't string");
					return m_str;
				}

			    result<trent::numer_type> trent::as_numer_critical() const {
					if (!is_numer()) return error("is't numer");
					return as_numer();
				}

			    result<std::map<std::string, trent>&> trent::as_dict_critical() {
			        if (!is_dict()) return error("is't dict");
			        return as_dict();
				}

			    result<const std::map<std::string, trent>&> trent::as_dict_critical() const {
			        if (!is_dict()) return error("is't dict");
			        return as_dict();
				}

			    trent::numer_type trent::as_numer_default(trent::numer_type def) const {
					if (!is_numer()) return def;
					return as_numer();
				}

				std::string& trent::as_string_default(std::string& def) {
					if (!is_string()) return def;
					return m_str;
				}

				bool trent::contains(owl::buffer buf) {
			        if (m_type != type::dict) {
						return false;
					}

					for(const auto& p : m_dict) {
						if (buf == owl::buffer(p.first.data(), p.first.size())) {
							return true;
						}
					}

					return false;
				}
		*/
	trent::type trent::get_type() const
	{
		return m_type;
	}

	const char * trent::type_to_str() const
	{
		switch (m_type)
		{
			case trent::type::string: 		return "string";
			case trent::type::list: 		return "list";
			case trent::type::dict:         return "dict";
			case trent::type::numer: 		return "numer";
			case trent::type::integer: 		return "integer";
			case trent::type::nil:          return "nil";
			default: abort();
		}
		return "";
	}
	/*
			trent& trent::operator= (const trent& other) {
				invalidate();
				m_type = other.m_type;
				switch(m_type) {
					case trent::type::string:
						owl::constructor(&m_str, other.m_str);
						return *this;
		            case trent::type::list:
						owl::constructor(&m_arr, other.m_arr);
						return *this;
		            case trent::type::dict:
						owl::constructor(&m_dict, other.m_dict);
		                return *this;
		            case trent::type::numer:
		                m_num = other.m_num;
						return *this;
		            case trent::type::integer:
		                m_int = other.m_int;
						return *this;
		            case trent::type::nil:
						return *this;
					default:
						PANIC_TRACED();
				}
				return *this;
			}

			trent& trent::operator= (const std::string& str) {
				reset(str);
				return *this;
			}

			trent& trent::operator= (owl::buffer buf) {
				reset(buf);
				return *this;
			}

			trent& trent::operator= (float num) {
				reset(num);
				return *this;
			}

			trent& trent::operator= (double num) {
				reset(num);
				return *this;
			}

			trent& trent::operator= (long double num) {
				reset(num);
				return *this;
			}

			trent& trent::operator= (signed char i){
				reset(i);
				return *this;
			}

			trent& trent::operator= (signed short i){
				reset(i);
				return *this;
			}

			trent& trent::operator= (signed int i){
				reset(i);
				return *this;
			}

			trent& trent::operator= (signed long i){
				reset(i);
				return *this;
			}

			trent& trent::operator= (signed long long i){
				reset(i);
				return *this;
			}


			trent& trent::operator= (unsigned char i){
				reset(i);
				return *this;
			}

			trent& trent::operator= (unsigned short i){
				reset(i);
				return *this;
			}

			trent& trent::operator= (unsigned int i){
				reset(i);
				return *this;
			}

			trent& trent::operator= (unsigned long i){
				reset(i);
				return *this;
			}

			trent& trent::operator= (unsigned long long i){
				reset(i);
				return *this;
			}

			trent& trent::operator= (bool i){
				reset(i);
				return *this;
			}

			ssize_t trent::size() {
		        switch(m_type) {
		            case trent::type::numer:
					case trent::type::string: return -1;
		            case trent::type::list: return m_arr.size();
		            case trent::type::dict: return m_dict.size();
		            default: PANIC_TRACED();
				}
				return 0;
			}
		*/
	size_t trent::print_to(nos::ostream& os) const
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
				nos::print_to(os, unsafe_string_const());
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
					nos::print_to(os, p.first);
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

		os.print("__error__");
		//unreacheble code
		return 0;
	}
	/*
			size_t trent_path_node::print_to(nos::ostream& o) const
			{
				if (is_string) return owl::print_to(o, str);
				return owl::print_to(o, i32);
			}
	*/
}

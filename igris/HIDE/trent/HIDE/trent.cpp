#include <igris/trent/trent.h>
/*#include <gxx/flow/keys.h>
#include <algorithm>

using namespace gxx::result_type;

namespace gxx {

	trent_basic::~trent_basic() {
		invalidate();
	}

	trent_basic::trent_basic() {}

	trent_basic::trent_basic(const trent_basic& other) {
		m_type = other.m_type;
		switch(m_type) {
			case trent_basic::type::string:
				gxx::constructor(&m_str, other.m_str);
				return;
            case trent_basic::type::list:
				gxx::constructor(&m_arr, other.m_arr);
				return;
            case trent_basic::type::dict:
				gxx::constructor(&m_dict, other.m_dict);
                return;
            case trent_basic::type::numer:
                m_num = other.m_num;
				return;
            case trent_basic::type::boolean:
            case trent_basic::type::integer:
                m_int = other.m_int;
				return;
            case trent_basic::type::nil:
				return;
			default:
				PANIC_TRACED();
		}
	}

	//trent_basic::trent_basic(const std::string& str) { init(str); }
	//trent_basic::trent_basic(const char* str) { init(str); }
	//trent_basic::trent_basic(const trent_basic::type& t) { init(t); }
    //trent_basic::trent_basic(const float& i) { init(i); }
    //trent_basic::trent_basic(const double& i) { init(i); }
    //trent_basic::trent_basic(const long double& i) { init(i); }
    //trent_basic::trent_basic(const signed char& i) { init(i); }
    //trent_basic::trent_basic(const signed short& i) { init(i); }
    //trent_basic::trent_basic(const signed int& i) { init(i); }
    //trent_basic::trent_basic(const signed long& i) { init(i); }
    //trent_basic::trent_basic(const signed long long& i) { init(i); }
    //trent_basic::trent_basic(const unsigned char& i) { init(i); }
    //trent_basic::trent_basic(const unsigned short& i) { init(i); }
    //trent_basic::trent_basic(const unsigned int& i) { init(i); }
    //trent_basic::trent_basic(const unsigned long& i) { init(i); }
    //trent_basic::trent_basic(const unsigned long long& i) { init(i); }

	void trent_basic::init(trent_basic::type t) {
		m_type = t;
		switch(m_type) {
			case trent_basic::type::string:
				gxx::constructor(&m_str);
				return;
            case trent_basic::type::list:
				gxx::constructor(&m_arr);
				return;
            case trent_basic::type::dict:
				gxx::constructor(&m_dict);
                return;
            case trent_basic::type::integer:
            case trent_basic::type::numer:
            case trent_basic::type::nil:
				return;
			default:
				PANIC_TRACED();
		}
	}
*/

/*
	void trent_basic::invalidate() {
		switch(m_type) {
			case trent_basic::type::string:
				gxx::destructor(&m_str);
				return;
            case trent_basic::type::list:
				gxx::destructor(&m_arr);
				return;
            case trent_basic::type::dict:
				gxx::destructor(&m_dict);
				return;
            case trent_basic::type::nil:
            case trent_basic::type::integer:
            case trent_basic::type::numer:
            case trent_basic::type::boolean:
				return;
			default:
				PANIC_TRACED();
				return;
		}
        m_type = trent_basic::type::nil;
	}

	trent_basic& trent_basic::operator[](int i) {
        if (m_type != trent_basic::type::list) init(trent_basic::type::list);
		if(m_arr.size() <= (unsigned int)i) m_arr.resize(i + 1);
		return m_arr[i];
	}

	const trent_basic& trent_basic::operator[](int key) const {
        if (m_type != trent_basic::type::list) gxx::panic("wrong trent_basic type");
		return m_arr.at(key);
	}

	trent_basic& trent_basic::operator[](const char* key) {
        if (m_type != trent_basic::type::dict) init(trent_basic::type::dict);
		return m_dict[key];
	}

	trent_basic& trent_basic::operator[](const std::string& key) {
        if (m_type != trent_basic::type::dict) init(trent_basic::type::dict);
		return m_dict[key];
	}

	const trent_basic& trent_basic::operator[](const std::string& key) const {
        if (m_type != trent_basic::type::dict) gxx::panic("wrong trent_basic type");
		return m_dict.at(key);
	}

	trent_basic& trent_basic::operator[](const gxx::buffer& key) {
        if (m_type != trent_basic::type::dict) init(trent_basic::type::dict);
		return m_dict[std::string(key.data(), key.size())];
	}

	const trent_basic& trent_basic::operator[] (const gxx::trent_basic_path& path) const {
		const gxx::trent_basic* tr = this;
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

        trent_basic& trent_basic::operator[] (const gxx::trent_basic_path& path) {
                gxx::trent_basic* tr = this;
                for (auto& p : path) {
                        if (p.is_string) {
                                tr = &tr->operator[](p.str);
                        }
                        else {
                                tr = &tr->operator[](p.i32);
                        }
                }
                return *tr;
        }

	const trent_basic& trent_basic::at(int i) const {
		if (m_type != trent_basic::type::list) gxx::panic("wrong trent_basic type");
        if(m_arr.size() <= (unsigned int)i) gxx::panic("wrong trent_basic list size");
		return m_arr[i];
	}


        const trent_basic& trent_basic::operator[](const char* key) const {
        if (m_type != trent_basic::type::dict) gxx::panic("wrong trent_basic type");
                return m_dict.at(key);
        }

        /*const trent_basic& trent_basic::at(const char* key) const {
        gxx::println(2);
        if (m_type != trent_basic::type::dict) init(trent_basic::type::dict);
                return m_dict[key];
        }*/

/*	const trent_basic& trent_basic::at(const std::string& key) const {
        if (m_type != trent_basic::type::dict) gxx::panic("wrong trent_basic type");
		return m_dict.at(key);
	}

	const trent_basic& trent_basic::at(const gxx::buffer& key) const {
        if (m_type != trent_basic::type::dict) gxx::panic("wrong trent_basic type");
		return m_dict.at(std::string(key.data(), key.size()));
	}

	bool trent_basic::have(const std::string& str) const {
        if (m_type != trent_basic::type::dict) gxx::panic("wrong trent_basic type");
		return m_dict.count(str) != 0;
	}

    std::map<std::string, trent_basic>& trent_basic::as_dict() {
        if (m_type != trent_basic::type::dict) init(trent_basic::type::dict);
		return m_dict;
	}

    const std::map<std::string, trent_basic>& trent_basic::as_dict() const {
        if (m_type != trent_basic::type::dict) gxx::panic("wrong_trent_basic_type");
		return m_dict;
	}

    std::vector<trent_basic>& trent_basic::as_list() {
        if (m_type != trent_basic::type::list) init(trent_basic::type::list);
		return m_arr;
	}

    const std::vector<trent_basic>& trent_basic::as_list() const {
        if (m_type != trent_basic::type::list) gxx::panic("wrong_trent_basic_type");
		return m_arr;
	}

    result<std::vector<trent_basic>&> trent_basic::as_list_critical() {
        if (!is_list()) return error("is't list");
		return m_arr;
	}
    result<const std::vector<trent_basic>&> trent_basic::as_list_critical() const {
        if (!is_list()) return error("is't list");
		return m_arr;
    }

	std::string& trent_basic::as_string() {
		if (m_type != trent_basic::type::string) init(trent_basic::type::string);
		return m_str;
	}
	const std::string& trent_basic::as_string() const {
		if (m_type != trent_basic::type::string) gxx::panic("wrong_trent_basic_type");
		return m_str;
	}

	const gxx::buffer trent_basic::as_buffer() const {
		if (m_type == trent_basic::type::string) return gxx::buffer(m_str.data(), m_str.size());
		return gxx::buffer();
	}

    trent_basic::numer_type trent_basic::as_numer() const {
		if (m_type == trent_basic::type::numer) return m_num;
		if (m_type == trent_basic::type::integer) return m_int;
		return 0;
	}

	trent_basic::integer_type trent_basic::as_integer() const {
		if (m_type == trent_basic::type::integer) return m_int;
		return 0;
	}

        result<trent_basic::integer_type>  trent_basic::as_integer_critical() const {
                if (!is_integer()) return error("is't integer");
                return m_int;
        }

	result<std::string&> trent_basic::as_string_critical() {
		if (!is_string()) return error("is't string");
		return m_str;
	}

	result<const std::string&> trent_basic::as_string_critical() const {
		if (!is_string()) return error("is't string");
		return m_str;
	}

    result<trent_basic::numer_type> trent_basic::as_numer_critical() const {
		if (!is_numer()) return error("is't numer");
		return as_numer();
	}

    result<std::map<std::string, trent_basic>&> trent_basic::as_dict_critical() {
        if (!is_dict()) return error("is't dict");
        return as_dict();
	}

    result<const std::map<std::string, trent_basic>&> trent_basic::as_dict_critical() const {
        if (!is_dict()) return error("is't dict");
        return as_dict();
	}

    trent_basic::numer_type trent_basic::as_numer_default(trent_basic::numer_type def) const {
		if (!is_numer()) return def;
		return as_numer();
	}

	std::string& trent_basic::as_string_default(std::string& def) {
		if (!is_string()) return def;
		return m_str;
	}

	bool trent_basic::contains(gxx::buffer buf) {
        if (m_type != type::dict) {
			return false;
		}

		for(const auto& p : m_dict) {
			if (buf == gxx::buffer(p.first.data(), p.first.size())) {
				return true;
			}
		}

		return false;
	}

	trent_basic::type trent_basic::get_type() const {
		return m_type;
	}

	const char * trent_basic::type_to_str() const {
		switch(m_type) {
            case trent_basic::type::string: 		return "string";
            case trent_basic::type::list: 		return "list";
            case trent_basic::type::dict:         return "dict";
            case trent_basic::type::numer: 		return "numer";
            case trent_basic::type::integer: 		return "integer";
            case trent_basic::type::nil:          return "nil";
			default: PANIC_TRACED();
		}
		return "";
	}

	trent_basic& trent_basic::operator= (const trent_basic& other) {
		invalidate();
		m_type = other.m_type;
		switch(m_type) {
			case trent_basic::type::string:
				gxx::constructor(&m_str, other.m_str);
				return *this;
            case trent_basic::type::list:
				gxx::constructor(&m_arr, other.m_arr);
				return *this;
            case trent_basic::type::dict:
				gxx::constructor(&m_dict, other.m_dict);
                return *this;
            case trent_basic::type::numer:
                m_num = other.m_num;
				return *this;
            case trent_basic::type::integer:
                m_int = other.m_int;
				return *this;
            case trent_basic::type::nil:
				return *this;
			default:
				PANIC_TRACED();
		}
		return *this;
	}

	trent_basic& trent_basic::operator= (const std::string& str) {
		reset(str);
		return *this;
	}

	trent_basic& trent_basic::operator= (gxx::buffer buf) {
		reset(buf);
		return *this;
	}

	trent_basic& trent_basic::operator= (float num) {
		reset(num);
		return *this;
	}

	trent_basic& trent_basic::operator= (double num) {
		reset(num);
		return *this;
	}

	trent_basic& trent_basic::operator= (long double num) {
		reset(num);
		return *this;
	}

	trent_basic& trent_basic::operator= (signed char i){
		reset(i);
		return *this;
	}

	trent_basic& trent_basic::operator= (signed short i){
		reset(i);
		return *this;
	}

	trent_basic& trent_basic::operator= (signed int i){
		reset(i);
		return *this;
	}

	trent_basic& trent_basic::operator= (signed long i){
		reset(i);
		return *this;
	}

	trent_basic& trent_basic::operator= (signed long long i){
		reset(i);
		return *this;
	}


	trent_basic& trent_basic::operator= (unsigned char i){
		reset(i);
		return *this;
	}

	trent_basic& trent_basic::operator= (unsigned short i){
		reset(i);
		return *this;
	}

	trent_basic& trent_basic::operator= (unsigned int i){
		reset(i);
		return *this;
	}

	trent_basic& trent_basic::operator= (unsigned long i){
		reset(i);
		return *this;
	}

	trent_basic& trent_basic::operator= (unsigned long long i){
		reset(i);
		return *this;
	}

	trent_basic& trent_basic::operator= (bool i){
		reset(i);
		return *this;
	}

	ssize_t trent_basic::size() {
        switch(m_type) {
            case trent_basic::type::numer:
			case trent_basic::type::string: return -1;
            case trent_basic::type::list: return m_arr.size();
            case trent_basic::type::dict: return m_dict.size();
            default: PANIC_TRACED();
		}
		return 0;
	}
}
*/
#ifndef GXX_GBSON_TOKEN_H
#define GXX_GBSON_TOKEN_H

#include <gxx/util/gbson.h>
#include <gxx/print.h>

namespace gxx { namespace gbson {

	struct token {
		gxx::buffer buf;
		int8_t type;

		token(gxx::buffer buf, uint8_t type) : buf(buf), type(type) {}

		//error
		token() : type(-1) {};  

		const char* strtype() const {
			switch (type) {
				case gbson_integer_type: return "gbson_integer_type";
				case gbson_float_type: return "gbson_float_type";
				case gbson_bytes_type: return "gbson_bytes_type";
				case gbson_array_type: return "gbson_array_type";
				case gbson_dictionary_type: return "gbson_dictionary_type";
				case -1: return "gbson_ERROR";
			}
		}
	};

	class token_reader {
		gxx::buffer buf;
		char* ptr;
		char* end;

	public:
		token_reader(gxx::buffer buf) : buf(buf), ptr(buf.data()), end(buf.data() + buf.size()) {}

		token integer_token() {
			uint8_t size = *(ptr + 1);
			token ret = token(gxx::buffer(ptr + 2, size), gbson_integer_type);
			ptr += size + 2;
			return ret;
		}

		token array_token() {
			token ret = token(gxx::buffer(ptr + 1, 1), gbson_array_type);
			ptr += 2;
			return ret;
		}

		token gettoken() {
			if(ptr >= end) return token();

			uint8_t type = *ptr;

			switch (type) {
				case gbson_integer_type:
					return integer_token();

				//case gbson_float_type:
				//	dprln("float_type");
				//		break;
	
				case gbson_array_type:
					return array_token();
	
				//case gbson_dictionary_type:
				//	dprln("dictionary_type");
				//	break;
			}

			return token();
		}
	};
}

	template<> struct print_functions<gxx::gbson::token> {
		static int print(gxx::io::ostream& o, gxx::gbson::token const& token) {
			gxx::println(token.strtype());
			debug_print_dump(token.buf.data(), token.buf.size());
			return 1;
		}		
	};

}

#endif
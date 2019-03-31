#include <nos/util/string.h>
#include <nos/util/hexascii.h>

namespace nos {
	strvec split(const std::string& str, char delim) {
		nos::strvec outvec;
	
		char* strt;
		char* ptr = (char*)str.data();
		char* end = (char*)str.data() + str.size();
		
		while(true) {
			while (*ptr == delim) ptr++;
			if (ptr == end) break;
		
			strt = ptr;
			while (*ptr != delim && ptr != end) ptr++;
			outvec.emplace_back(strt, ptr - strt);		
		}
	
		return outvec;
	}

	std::string join(const nos::strvec& vec, char delim) {
		if (vec.size() == 0) {
			return "";
		}

		std::string ret;

		size_t len = 0;
		for (auto& s : vec) {
			len ++;
			len += s.size();  
		}
		ret.reserve(len);

		auto preend = vec.end();
		auto iter = vec.begin();
		preend--;
		for (; iter != preend; iter++) {
			ret.append(*iter);
			ret.push_back(delim);
		}
		ret.append(*iter);

		return ret;
	}  
	
	std::string dstring(const void* data, size_t size) {
		std::string ret;
		char* it = (char*) data;
		char* eit = it + size;

		for (;it!=eit;++it) {
			if (isprint(*it)) ret.push_back(*it);
			else if (*it == '\n') ret.append("\\n", 2);
			else if (*it == '\t') ret.append("\\t", 2);
			else if (*it == '\\') ret.append("\\\\", 2);
			else { 
				char hi = half2hex((uint8_t)((*it & 0xF0) >> 4));
				char low = half2hex((uint8_t)(*it & 0x0F));
				ret.append("\\x", 2);
				ret.push_back(hi);  
				ret.push_back(low); 
			}
		}
		return ret;
	}

	/*std::string dstring(nod::buffer buf) {
		return dstring(buf.data(), buf.size());
	}*/	

	std::string dstring(const std::string& buf) {
		return dstring(buf.data(), buf.size());
	}	
}
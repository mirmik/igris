#ifndef IGRIS_BUFFER_H
#define IGRIS_BUFFER_H

#include <string>

#include <string.h>
#include <stdlib.h>

#include <igris/util/setget.h>

namespace igris
{
	class buffer
	{
	protected:
		char* buf;
		size_t sz;

	public:
		char * data() const { return buf; }
		char *& data() { return buf; }
		void data(char* buf) { this->buf = buf; }

		size_t size() const { return sz; }
		size_t& size() { return sz; }
		void size(size_t sz) { this->sz = sz; }

	//ctors:
		buffer() : buf(nullptr), sz(0) {}

		buffer(const char* _buf) : buf((char*)_buf), sz(strlen(_buf)) {}

		buffer(const void* _buf, size_t _sz) : buf((char*)_buf), sz(_sz) {}
		buffer(const std::string& str) : buffer(str.data(), str.size()) {}

		template<size_t N>
		inline buffer(const char (&arr) [N]) : buf((char*)arr), sz(N) {}

		explicit operator std::string() 
		{
			return std::string(buf, sz);
		}

	//methods:
		bool operator==(const buffer& other) const
		{
			return (sz == other.sz) && (strncmp(buf, other.buf, sz < other.sz ? sz : other.sz) == 0);
		}

		bool operator!=(const buffer& other) const
		{
			return (sz != other.sz) || (strncmp(buf, other.buf, sz < other.sz ? sz : other.sz) != 0);
		}

		bool operator==(const char* str)
		{
			return strncmp(buf, str, sz) == 0;
		}

		bool operator!=(const char* str)
		{
			return strncmp(buf, str, sz) != 0;
		}

		char& operator[](size_t num)
		{
			return *(buf + num);
		}

		char operator[](size_t num) const
		{
			return *(buf + num);
		}

		char * operator * ()
		{
			return buf;
		}

		size_t operator + ()
		{
			return sz;
		}

		char* begin()
		{
			return buf;
		}

		char* end()
		{
			return buf + sz;
		}

		bool empty()
		{
			return buf == nullptr;
		}

		buffer slice(size_t idx, size_t _sz)
		{
			return buffer(buf + idx, _sz);
		}

		template<typename T>
		static igris::buffer on_object(T& obj)
		{
			return buffer((char*)&obj, sizeof(obj));
		}
	};

	namespace buffer_literal
	{
		inline buffer operator"" _b(const char* str, size_t sz) { return buffer(str, sz); }
	}
}

#endif

#ifndef OWL_UTILXX_BUFFER_H
#define OWL_UTILXX_BUFFER_H

#include <string>
#include <string.h>
#include <stdlib.h>

namespace owl
{
	class buffer
	{
	protected:
		char* _data;
		size_t _size;

	public:
		//accessors:
		void data(void* arg) { _data = (char*)arg; }
		void size(size_t arg) { _size = arg; }

		char* data() const { return _data; }
		size_t size() const { return _size; }

		//ctors:
		buffer() : _data(nullptr), _size(0) {}

		buffer(const char* _buf) : _data((char*)_buf), _size(strlen(_buf)) {}

		buffer(const void* _buf, size_t _sz) : _data((char*)_buf), _size(_sz) {}

		buffer(const std::string& str) : _data((char*)str.data()), _size(str.size()) {}

		template<size_t N>
		inline buffer(const char (&arr) [N]) : _data((char*)arr), _size(N) {}

		//methods:
		bool operator==(const buffer& other) const
		{
			return (_size == other._size) && (strncmp(_data, other._data, _size < other._size ? _size : other._size) == 0);
		}

		bool operator!=(const buffer& other) const
		{
			return (_size != other._size) || (strncmp(_data, other._data, _size < other._size ? _size : other._size) != 0);
		}

		bool operator==(const char* str)
		{
			return strncmp(_data, str, _size) == 0;
		}

		bool operator!=(const char* str)
		{
			return strncmp(_data, str, _size) != 0;
		}

		char& operator[](size_t num)
		{
			return *(_data + num);
		}

		char operator[](size_t num) const
		{
			return *(_data + num);
		}

		char * operator * ()
		{
			return _data;
		}

		size_t operator + ()
		{
			return _size;
		}

		char* begin()
		{
			return _data;
		}

		char* end()
		{
			return _data + _size;
		}

		bool empty()
		{
			return _data == nullptr;
		}
	};
}

#endif

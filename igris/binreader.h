#ifndef IGRIS_BINREADER_H
#define IGRIS_BINREADER_H

#include <igris/util/numconvert.h>

namespace igris 
{
	class binreader 
	{
		const char * ptr;

	public:
		binreader (){}
		binreader (const char * ptr) : ptr(ptr) {}

		void init(const char * ptr) { this->ptr = ptr; }

		template <class T> 
		void read_binary(T& obj) 
		{
			memcpy(&obj, ptr, sizeof(T));
			ptr += sizeof(T);
		} 

		template <class Pointer>
		void bind_buffer(Pointer*& data, size_t sz) 
		{
			data = (Pointer*) ptr;
			ptr += sz;
		}

		void bind_buffer(igris::buffer& data, size_t sz) 
		{
			data = { ptr, sz };
			ptr += sz;
		}

		void skip(int n) 
		{
			ptr += n;
		}

		int read_ascii_decimal_integer(int * ret) 
		{
			*ret = atoi32(ptr, 10, (char**)&ptr);
			return 0;
		}

		int read_ascii_decimal_float(float * ret) 
		{
			*ret = atof32(ptr, (char**)&ptr);
			return 0;
		}
	};
}

#endif
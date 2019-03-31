#ifndef IGRIS_POOL_H
#define IGRIS_POOL_H

#include <igris/datastruct/pool.h>

namespace igris
{
	class pool 
	{
		struct pool_head head;
		void* _zone;
		size_t _size;
		size_t _elemsz;

	public:

		pool(){}

		pool(void* zone, size_t size, size_t elsize) {
			init(zone, size, elsize);
		}

		void init(void* zone, size_t size, size_t elsize) 
		{
			_zone = zone;
			_size = size;
			_elemsz = elsize;
			pool_init(&head);
			pool_engage(&head, zone, size, elsize);
		}

		void* get() 
		{
			void* ret = pool_alloc(&head);
			return ret;
		}

		void put( void* ptr ) 
		{
			assert((uintptr_t)ptr >= (uintptr_t)_zone);
			assert((uintptr_t)ptr < (uintptr_t)_zone + _size);

			pool_free(&head, ptr);
		}

		size_t size() const 
		{
			return _size / _elemsz;
		}

		size_t element_size() const 
		{
			return _elemsz;
		}

		size_t avail() const 
		{
			return pool_avail(&head);
		}
	};
}

#endif
#ifndef GXX_BUFFER_HEAD_H
#define GXX_BUFFER_HEAD_H

#include <stdint.h>

struct buffer_head 
{
	void * data;
	size_t size;
};

__BEGIN_DECLS 

static inline void buffer_init(struct buffer_head * head, 
		void * data, size_t size) 
{
	head -> data = data;
	head -> size = size;
}

__END_DECLS

#endif
#ifndef GXX_DATASTRUCT_BUF_H
#define GXX_DATASTRUCT_BUF_H

typedef struct buf_s {
	char* data;
	unsigned int size;
} buf_t;

typedef struct cbuf_s {
	const char* data;
	unsigned int size;
} cbuf_t;

#endif
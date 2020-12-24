#ifndef IGRIS_DATASTRUCT_SLINE_H
#define IGRIS_DATASTRUCT_SLINE_H

#include <igris/dprint.h>
#include <igris/compiler.h>
#include <string.h>

struct sline
{
	char* buf;
	int cap;
	int len;
	int cursor;
};

__BEGIN_DECLS

static inline
const char* sline_getline(struct sline * sl) 
{
	sl->buf[sl->len] = '\0';	
	return sl->buf;
}

static inline 
char* sline_rightpart(struct sline * sl) 
{
	return sl->buf + sl->cursor;
}

static inline 
unsigned int sline_rightsize(struct sline * sl) 
{
	return sl->len - sl->cursor;
}

static inline 
unsigned int sline_in_rightpos(struct sline * sl) 
{
	return sl->len == sl->cursor;
}


static inline 
void sline_reset(struct sline * sl)
{
	sl->len = 0;
	sl->cursor = 0;
}

static inline 
int sline_equal(struct sline * sl, const char * str) 
{
	if (sl->len != (int)strlen(str)) 
		return 0;
	
	return strncmp(sl->buf, str, sl->len) == 0;
} 

static inline 
int sline_left(struct sline * sl) 
{
	if (sl->cursor == 0) return 0;
	--sl->cursor;
	return 1;
}

static inline 
int sline_right(struct sline * sl) 
{
	if (sl->cursor == sl->len) return 0;
	++sl->cursor;
	return 1;
}

static inline 
void sline_setbuf(struct sline * sl, char* buffer, int bufcap)
{
	sl->buf = buffer;
	sl->cap = bufcap;
}

static inline 
void sline_init(struct sline * sl, char* buffer, int bufcap)
{
	sline_setbuf(sl, buffer, bufcap);
	sline_reset(sl);
}


static inline 
int sline_backspace(struct sline * sl, int count) 
{
	if (count > sl->cursor) 
		count = sl->cursor;

	sl->len -= count;
	sl->cursor -= count;

	if (sl->cursor != sl->len) 
	{
		//dprln("move", sl->len - sl->cursor);
		memmove(sl->buf + sl->cursor, sl->buf + sl->cursor + count, 
			sl->len - sl->cursor);
	}

	return count;
}

static inline 
int sline_delete(struct sline * sl, unsigned int count) 
{
	if (count > sline_rightsize(sl)) 
		count = sline_rightsize(sl);

	sl->len -= count;

	if (sl->cursor != sl->len) 
	{
		memmove(sl->buf + sl->cursor, sl->buf + sl->cursor + count, 
			sl->len - sl->cursor);
	}

	return count;
}

static inline 
int sline_empty(struct sline * sl)
{
	return sl->len == 0;
}

static inline 
int sline_avail(struct sline * sl)
{
	return sl->cap - sl->len;
}

static inline 
int sline_putchar(struct sline * sl, char c)
{
	if (sl->len >= sl->cap)
		return 0;
	
	if (sl->cursor != sl->len) 
	{
		memmove(sl->buf + sl->cursor + 1, sl->buf + sl->cursor, 
			sl->len - sl->cursor);
	}

	sl->buf [ sl->cursor++ ] = c;
	sl->len++;	

	return 1;
}

/*static inline 
int sline_write(struct sline * sl, const char* dat, int sz)
{
	int av = sline_avail(sl);
	int cap = sz < av ? sz : av;

	memcpy(sl->buf + sl->len, dat, cap);
	sl->len += cap;
	return cap;
}*/

static inline 
void sline_debug_info(struct sline * sl) 
{
	debug_print("buffer:"); debug_printhex_ptr(sl->buf); dln();	
	debug_print("capacity:"); debug_printdec_signed_int(sl->cap); dln();
	debug_print("length:"); debug_printdec_signed_int(sl->len); dln();
	debug_print("cursor:"); debug_printdec_signed_int(sl->cursor); dln();
}

__END_DECLS

#endif

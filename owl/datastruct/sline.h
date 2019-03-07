#ifndef GXX_DATASTRUCT_SLINE_H
#define GXX_DATASTRUCT_SLINE_H

#include <sys/cdefs.h>
#include <string.h>

struct sline 
{
	char* buf;
	int cap;
	int len;
};

__BEGIN_DECLS

static inline void
sline_reset(struct sline * sl) 
{
	sl->len = 0;
}

static inline void 
sline_setbuf(struct sline * sl, char* buffer, int bufcap) 
{
	sl->buf = buffer;
	sl->cap = bufcap;
}

static inline void 
sline_init(struct sline * sl, char* buffer, int bufcap) 
{
	sline_setbuf(sl, buffer, bufcap);
	sline_reset(sl);
}

static inline int
sline_back(struct sline * sl, int n) 
{
	int back = n > sl->len ? sl->len : n;
	sl->len -= back;
	return back;
}

static inline int
sline_empty(struct sline * sl) 
{
	return sl->len == 0;
}

static inline int
sline_avail(struct sline * sl) 
{
	return sl->cap - sl->len;
}

static inline int
sline_putchar(struct sline * sl, char c) 
{
	if (sl->len >= sl->cap) 
		return 0;

	sl->buf [ sl->len++ ] = c;
	return 1;
}

static inline int
sline_write(struct sline * sl, const char* dat, int sz) {
	int av = sline_avail(sl);
	int cap = sz < av ? sz : av;

	memcpy(sl->buf + sl->len, dat, cap);
	sl->len += cap;
	return cap;
}

__END_DECLS

#endif
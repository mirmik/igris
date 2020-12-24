#include <igris/string.h>
#include <string.h>
#include <igris/math/defs.h>

#include <stdio.h>

void replace_substrings(
    char * buffer, size_t maxsize,
    const char * input, size_t inlen,
    const char * sub, size_t sublen,
    const char * rep, size_t replen
)
{
	const char * strit = input;
	const char * streit = input + inlen;
	char * bufit = buffer;

	if (sublen == 0)
	{
		memcpy(buffer, input, __MIN__(maxsize, inlen));
	}

	char * finded;
	while ((finded = igris_memmem(strit, streit - strit, sub, sublen)) != NULL)
	{
		int step = finded - strit;

		memcpy(bufit, strit, step);
		bufit += step;
		strit += step;

		memcpy(bufit, rep, replen);
		bufit += replen;
		strit += sublen;	
	};

	memcpy(bufit, strit, streit - strit);
}
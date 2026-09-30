#include <igris/math/defs.h>
#include <igris/util/string.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

void replace_substrings(char *buffer,
                        size_t maxsize,
                        const char *input,
                        size_t inlen,
                        const char *sub,
                        size_t sublen,
                        const char *rep,
                        size_t replen)
{
    if (buffer == NULL || maxsize == 0)
        return;

    const char *strit = input;
    const char *streit = input + inlen;
    char *bufit = buffer;
    size_t room = maxsize - 1;

    if (sublen == 0)
    {
        size_t len = __MIN__(room, inlen);
        memmove(buffer, input, len);
        buffer[len] = 0;
        return;
    }

    char *finded;
    while ((finded = igris_memmem(strit, streit - strit, sub, sublen)) != NULL)
    {
        ptrdiff_t step = finded - strit;

        size_t copy = __MIN__(room, (size_t)step);
        memmove(bufit, strit, copy);
        bufit += copy;
        room -= copy;
        strit += step;

        copy = __MIN__(room, replen);
        memmove(bufit, rep, copy);
        bufit += copy;
        room -= copy;
        strit += sublen;
    };

    ptrdiff_t lastlen = streit - strit;
    size_t copy = __MIN__(room, (size_t)lastlen);
    memmove(bufit, strit, copy);
    bufit[copy] = 0;
}

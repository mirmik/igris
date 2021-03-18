#include <igris/binreader.h>
#include <string.h>

void reader_memcpy(const char **ptr, void *obj, size_t objsize)
{
    memcpy(obj, *ptr, objsize);
    *ptr += objsize;
}

void reader_skip(const char **ptr, size_t size) { *ptr += size; }

void reader_skip_while(const char **ptr, char c, const char *protector)
{
    while (*ptr != protector && **ptr == c)
        ++*ptr;
}

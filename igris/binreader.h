/**
    Однопроходной парсинг последовательных данных.
*/

#ifndef IGRIS_BINREADER_H
#define IGRIS_BINREADER_H

#include <igris/compiler.h>
#include <stddef.h>
#include <string.h>

__BEGIN_DECLS

void reader_memcpy(const char **ptr, void *obj, size_t objsize);
void reader_skip(const char **ptr, size_t size);
void reader_skip_while(const char **ptr, char c, const char *protector);

__END_DECLS

#ifdef __cplusplus
#include <igris/util/numconvert.h>
#include <string_view>

namespace igris
{
    class binreader
    {
        const char *ptr;

      public:
        binreader() {}
        binreader(const char *ptr) : ptr(ptr) {}

        void init(const char *ptr) { this->ptr = ptr; }

        template <class T> void read_binary(T &obj)
        {
            memcpy(&obj, ptr, sizeof(T));
            ptr += sizeof(T);
        }

        template <class Pointer> void bind_buffer(Pointer *&data, size_t sz)
        {
            data = (Pointer *)ptr;
            ptr += sz;
        }

        void bind_buffer(std::string_view &data, size_t sz)
        {
            data = {ptr, sz};
            ptr += sz;
        }

        void skip(int n) { ptr += n; }

        int read_ascii_decimal_integer(int *ret)
        {
            *ret = atoi32(ptr, 10, (char **)&ptr);
            return 0;
        }

        int read_ascii_decimal_float(float *ret)
        {
            *ret = atof32(ptr, (char **)&ptr);
            return 0;
        }
    };
}
#endif

#endif

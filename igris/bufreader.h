#ifndef IGRIS_READER_H
#define IGRIS_READER_H

#include <igris/compiler.h>
#include <stdint.h>
#include <string.h>

/*struct bufreader_token
{
    const char *data;
    int size;
};*/

struct bufreader
{
    const char *strt;
    const char *fini;

    const char *cursor;
};

__BEGIN_DECLS

static inline void bufreader_init(struct bufreader *reader, const char *strt,
                                  int size)
{
    reader->strt = reader->cursor = strt;
    reader->fini = strt + size;
}

static inline uint8_t bufreader_end(struct bufreader *reader)
{
    return reader->cursor == reader->fini;
}

static inline int bufreader_readline(struct bufreader *reader,
                                     const char **token)
{
    int len;
    const char *it = reader->cursor;

    if (bufreader_end(reader))
        return -1;

    while (*it != '\n' && *it != '\0' && it != reader->fini)
        it++;

    *token = reader->cursor;

    if (it != reader->fini)
    {
        reader->cursor = it + 1;
    }
    else
    {
        reader->cursor = reader->fini;
    }

    while (*it == '\n' || *it == '\r')
        --it;

    len = it - *token + 1;
    return len;
}

__END_DECLS

#endif
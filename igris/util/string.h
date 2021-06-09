#ifndef IGRIS_STRING_H
#define IGRIS_STRING_H

#include <cstdint>
#include <ctype.h>
#include <list>
#include <string>
#include <vector>

#if __cplusplus > 201500L
#include <string_view>
#endif

#include <igris/buffer.h>
#include <igris/util/hexascii.h>

__BEGIN_DECLS

void *igris_memmem(const void *l, size_t l_len, const void *s, size_t s_len);

void replace_substrings(char *buffer, size_t maxsize, const char *input,
                        size_t inlen, const char *sub, size_t sublen,
                        const char *rep, size_t replen);

__END_DECLS

namespace igris
{
    std::string replace(const std::string &src, const std::string &oldsub,
                        const std::string &newsub);

    std::vector<std::string> split(const igris::buffer &str, char delim = ' ');
    std::vector<std::string> split(const igris::buffer &str,
                                   const char *delims);

    std::string join(const std::vector<std::string> &, char delim);

    template <class Iter>
    std::string join(Iter start, Iter end, const char *delim,
                     const char *prefix, const char *postfix)
    {
        std::string ret;

        size_t sz = 0;
        size_t tot = 0;

        for (Iter it = start; it != end; ++it)
        {
            sz += it->size();
            ++tot;
        }

        ret.reserve(sz + strlen(prefix) + strlen(postfix) +
                    tot * strlen(delim));

        ret.append(prefix);
        Iter it = start;
        for (unsigned int i = 0; i < tot; ++i)
        {
            ret.append(*it++);
            ret.append(delim);
        }
        ret.append(*it);
        ret.append(postfix);

        return ret;
    }

#if __cplusplus > 201500L
    static inline std::string_view trim(const std::string_view &view)
    {
        if (view.size() == 0)
            return view;

        size_t strt = view.find_first_not_of(" \n\t");
        size_t fini = view.find_last_not_of(" \n\t") + 1;
        return view.substr(strt, fini - strt);
    }

    static inline std::string_view trim_left(const std::string_view &view)
    {
        if (view.size() == 0)
            return view;

        size_t fini = view.find_last_not_of(" \n\t") + 1;
        return view.substr(0, fini);
    }

    static inline std::string_view trim_right(const std::string_view &view)
    {
        if (view.size() == 0)
            return view;

        size_t strt = view.find_first_not_of(" \n\t");
        return view.substr(strt, view.size() - strt);
    }
#endif
}

#endif

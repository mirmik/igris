#ifndef IGRIS_USTIL_GRAYCODE_H
#define IGRIS_USTIL_GRAYCODE_H

namespace igris
{
    template <class T> static inline T graycode(const T &code)
    {
        return code ^ (code >> 1);
    }
} // namespace igris

#endif

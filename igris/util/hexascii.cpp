#include <igris/util/hexascii.h>

namespace igris
{
    std::string hexascii_encode(const uint8_t *indata, size_t size)
    {
        std::string ret;
        ret.resize(size * 2);
        const uint8_t *eit = indata + size;
        uint8_t *oit = (uint8_t *)&ret[0];
        for (const uint8_t *it = indata; it != eit; ++it)
        {
            *oit++ = half2hex((*it & 0xF0) >> 4);
            *oit++ = half2hex(*it & 0xF);
        }
        return ret;
    }

    std::string hexascii_encode(const std::string &str)
    {
        return hexascii_encode((const uint8_t *)str.data(), str.size());
    }

    std::string hexascii_encode(const std::string_view &buf)
    {
        return hexascii_encode((const uint8_t *)buf.data(), buf.size());
    }
}

void hexascii_encode(const void *indata, int size, void* out)
{
    const uint8_t *data = (uint8_t*)indata;
    const uint8_t *eit = (uint8_t*)indata + size;
    uint8_t *oit = (uint8_t*)out;
    
    for (const uint8_t *it = data; it != eit; ++it)
    {
        *oit++ = half2hex((*it & 0xF0) >> 4);
        *oit++ = half2hex(*it & 0xF);
    }

    *oit = 0;
}
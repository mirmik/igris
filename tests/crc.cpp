#include <doctest/doctest.h>
#include <igris/util/crc.h>
#include <cstring>

TEST_CASE("crc32_ciit") 
{
    int32_t ret = igris_crc32("HelloWorld", 10, 0);     
    CHECK_EQ(ret, 1114288986);
}

TEST_CASE("crc32 short and unaligned inputs")
{
    const unsigned char input[] = {0xff, 1, 2, 3, 4, 5, 6, 7, 8};
    for (uint32_t size = 0; size <= 7; ++size)
    {
        unsigned char aligned[8] = {};
        memcpy(aligned, input + 1, size);
        CHECK_EQ(igris_crc32(input + 1, size, 0x12345678),
                 igris_crc32(aligned, size, 0x12345678));
    }
}

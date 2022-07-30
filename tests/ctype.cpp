#include <cctype>
#include <cstdint>
#include <igris/util/ctype.h>
#include <doctest/doctest.h>
#include <cstdio>

TEST_CASE("ctype") 
{
    for (uint16_t c = 0; c < 256; c++) 
    {
        CHECK_EQ((bool)isalpha(c), igris_isalpha(c));
        CHECK_EQ((bool)isdigit(c), igris_isdigit(c));
        CHECK_EQ((bool)isspace(c), igris_isspace(c));
        CHECK_EQ((bool)isupper(c), igris_isupper(c));
        CHECK_EQ((bool)islower(c), igris_islower(c));
        CHECK_EQ((bool)isblank(c), igris_isblank(c));
        CHECK_EQ((bool)isprint(c), igris_isprint(c));
        CHECK_EQ((bool)isxdigit(c), igris_isxdigit(c));
    }
}
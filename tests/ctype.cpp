#include <cctype>
#include <cstdint>
#include <cstdio>
#include <doctest/doctest.h>
#include <igris/util/ctype.h>


TEST_CASE("ctype")
{
    for (uint16_t c = 0; c < 256; c++)
    {
        CHECK_EQ(isalpha(c), igris_isalpha(c));
        CHECK_EQ(isdigit(c), igris_isdigit(c));
        CHECK_EQ(isspace(c), igris_isspace(c));
        CHECK_EQ(isupper(c), igris_isupper(c));
        CHECK_EQ(islower(c), igris_islower(c));
        CHECK_EQ(isblank(c), igris_isblank(c));
        CHECK_EQ(isprint(c), igris_isprint(c));
        CHECK_EQ(isxdigit(c), igris_isxdigit(c));
    }
}
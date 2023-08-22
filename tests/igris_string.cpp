#include <doctest/doctest.h>
#include <igris/container/std_portable.h>

TEST_CASE("string2")
{
    igris::string str("hello lalala jjj");
    auto spl = str.split(' ');
    CHECK_EQ(spl[0], "hello");
    CHECK_EQ(spl[1], "lalala");
    CHECK_EQ(spl[2], "jjj");

    str.append(" kkk");
    spl = str.split(' ');
    CHECK_EQ(spl[0], "hello");
    CHECK_EQ(spl[1], "lalala");
    CHECK_EQ(spl[2], "jjj");
    CHECK_EQ(spl[3], "kkk");
}
#include <doctest/doctest.h>
#include <compat/std/memory>
#include <compat/std/string>
#include <compat/std/vector>

namespace
{
    struct owned_value
    {
        static int alive;
        int value;
        owned_value(int value) : value(value) { ++alive; }
        ~owned_value() { --alive; }
    };
    int owned_value::alive = 0;
}

TEST_CASE("igris_std string boundary and aliasing operations")
{
    igris_std::string str("abc");
    str += str;
    CHECK_EQ(str, "abcabc");
    CHECK_EQ(str.find("abc", 3), 3);
    CHECK_EQ(str.find(""), 0);
    CHECK_EQ(str.find("abcabc"), 0);
    CHECK_EQ(str.substr(4, 100), "bc");
    CHECK(str.substr(100, 2).empty());

    const char lhs_data[] = {'a', '\0', 'b'};
    const char rhs_data[] = {'a', '\0', 'c'};
    igris_std::string lhs(lhs_data, sizeof(lhs_data));
    igris_std::string rhs(rhs_data, sizeof(rhs_data));
    CHECK(lhs < rhs);
}

TEST_CASE("igris_std unique_ptr move assignment releases old object")
{
    owned_value::alive = 0;
    {
        igris_std::unique_ptr<owned_value> lhs(new owned_value(1));
        igris_std::unique_ptr<owned_value> rhs(new owned_value(2));
        lhs = static_cast<igris_std::unique_ptr<owned_value> &&>(rhs);
        CHECK_EQ(owned_value::alive, 1);
        CHECK_EQ(lhs->value, 2);
        CHECK_FALSE(rhs);
        lhs.reset(lhs.get());
        CHECK_EQ(owned_value::alive, 1);
    }
    CHECK_EQ(owned_value::alive, 0);
}

TEST_CASE("igris_std vector alias copies into correctly sized storage")
{
    igris_std::vector<int> source;
    source.push_back(1);
    source.push_back(2);
    igris_std::vector<int> target;
    target.push_back(9);
    target = source;
    CHECK_EQ(target.size(), 2);
    CHECK_EQ(target[1], 2);
}

#include <doctest/doctest.h>
#include <igris/container/static_vector.h>

namespace
{
    struct static_vector_tracked
    {
        static int alive;
        int value = 0;
        static_vector_tracked(int value = 0) : value(value) { ++alive; }
        static_vector_tracked(const static_vector_tracked &oth)
            : value(oth.value) { ++alive; }
        static_vector_tracked(static_vector_tracked &&oth) noexcept
            : value(oth.value) { ++alive; }
        static_vector_tracked &operator=(const static_vector_tracked &) = default;
        static_vector_tracked &operator=(static_vector_tracked &&) = default;
        ~static_vector_tracked() { --alive; }
    };
    int static_vector_tracked::alive = 0;
}

TEST_CASE("static_vector")
{
    igris::static_vector<int, 15> vec;

    vec.emplace_back(33);
    vec.emplace_back(22);
    vec.emplace_back(11);

    CHECK_EQ(vec[0], 33);
    CHECK_EQ(vec[1], 22);
    CHECK_EQ(vec[2], 11);

    CHECK_EQ(vec.room(), 15 - 3);
}


TEST_CASE("static_vector object lifetime")
{
    static_vector_tracked::alive = 0;
    {
        igris::static_vector<static_vector_tracked, 4> source;
        source.emplace_back(1);
        source.emplace_back(2);
        source.emplace_back(3);

        igris::static_vector<static_vector_tracked, 4> target;
        target.emplace_back(9);
        target = source;
        target.erase(target.begin() + 1, target.end());
        target.resize(3);
        target.resize(1);
        target.clear();
    }
    CHECK_EQ(static_vector_tracked::alive, 0);
}

TEST_CASE("static_vector initializer list is bounded by capacity")
{
    igris::static_vector<int, 2> values{1, 2, 3, 4};
    CHECK_EQ(values.size(), 2);
    CHECK_EQ(values[1], 2);
}

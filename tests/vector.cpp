#include <doctest/doctest.h>
#include <igris/container/vector.h>

TEST_CASE("vector")
{
    igris::vector<double> vec;

    vec.emplace_back(15.5);
    vec.emplace_back(15.25);
    vec.emplace_back(3);
    vec.emplace_back(2);

    CHECK_EQ(vec[0], 15.5);
    CHECK_EQ(vec[1], 15.25);
    CHECK_EQ(vec[2], 3);
    CHECK_EQ(vec[3], 2);

    vec.clear();

    vec.emplace_back(15.5);
    vec.emplace_back(15.25);
    vec.emplace_back(3);
    vec.emplace_back(2);

    CHECK_EQ(vec[0], 15.5);
    CHECK_EQ(vec[1], 15.25);
    CHECK_EQ(vec[2], 3);
    CHECK_EQ(vec[3], 2);
}

TEST_CASE("vector.push_back")
{
    igris::vector<double> vec;

    vec.push_back(15.5);
    vec.push_back(15.25);
    vec.push_back(3);
    vec.push_back(2);

    CHECK_EQ(vec[0], 15.5);
    CHECK_EQ(vec[1], 15.25);
    CHECK_EQ(vec[2], 3);
    CHECK_EQ(vec[3], 2);

    vec.clear();

    vec.push_back(15.5);
    vec.push_back(15.25);
    vec.push_back(3);
    vec.push_back(2);
    for (int i = 0; i < 1000; ++i)
        vec.push_back(2);

    CHECK_EQ(vec[0], 15.5);
    CHECK_EQ(vec[1], 15.25);
    CHECK_EQ(vec[2], 3);
    CHECK_EQ(vec[3], 2);
}

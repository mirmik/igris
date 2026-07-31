#include <doctest/doctest.h>
#include <igris/container/unbounded_array.h>
#include <vector>

TEST_CASE("igris_test_suite")
{
    igris::unbounded_array<float> arr(35);
    arr[28] = 33;
    CHECK_EQ(arr[28], 33);
}

TEST_CASE("unbounded array : vector")
{
    igris::unbounded_array<std::vector<double>> arr(4);

    arr[0].push_back(1);
    arr[0].push_back(2);

    arr[1].resize(2);
    arr[1][0] = 3;

    CHECK_EQ(arr[0][0], 1);
    CHECK_EQ(arr[0][1], 2);
    CHECK_EQ(arr[1][0], 3);
    CHECK_EQ(arr[2].size(), 0);
}

TEST_CASE("unbounded array copy assignment and resize")
{
    igris::unbounded_array<std::vector<int>> source(2);
    source[0].push_back(42);

    igris::unbounded_array<std::vector<int>> target(1);
    target[0].push_back(7);
    target = source;
    CHECK_EQ(target.size(), 2);
    CHECK_EQ(target[0][0], 42);

    target.resize(3);
    target[2].push_back(11);
    CHECK_EQ(target[2][0], 11);
}

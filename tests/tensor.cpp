#include <doctest/doctest.h>
#include <igris/container/tensor.h>

TEST_CASE("marray")
{
    igris::tensor<double> arr;

    arr.reshape({2, 2});
    arr({0, 0}) = 1;
    arr({0, 1}) = 2;
    arr({1, 0}) = 3;
    arr({1, 1}) = 4;

    CHECK_EQ(arr.storage_size(), 4);
    CHECK_EQ(arr.stride()[0], 2);
    CHECK_EQ(arr.stride()[1], 1);

    {
        auto subarray = arr[0];
        CHECK_EQ(subarray.storage_size(), 2);
        CHECK_EQ(subarray.shape().size(), 1);
        CHECK_EQ(subarray.stride().size(), 1);
        CHECK_EQ(subarray({0}), 1);
        CHECK_EQ(subarray({1}), 2);
    }

    {
        auto subarray = arr[1];
        CHECK_EQ(subarray.storage_size(), 2);
        CHECK_EQ(subarray.shape().size(), 1);
        CHECK_EQ(subarray.stride().size(), 1);
        CHECK_EQ(subarray({0}), 3);
        CHECK_EQ(subarray({1}), 4);
    }
}
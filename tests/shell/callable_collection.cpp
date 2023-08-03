#include <array>
#include <doctest/doctest.h>
#include <igris/container/static_vector.h>
#include <igris/shell/callable_collection.h>

TEST_CASE("callable_collection")
{
    igris::callable_collection<int(int, int)> cc;

    cc.add("sum", "Add two numbers", [](int a, int b) -> int { return a + b; });

    auto *fptr = cc.find("sum");
    CHECK((*fptr)(1, 2) == 3);
}

TEST_CASE("callable_collection with array")
{
    igris::callable_collection<
        int(int, int),
        igris::static_vector<igris::callable_collection_record<int(int, int)>,
                             5>>
        cc;

    cc.add("sum", "Add two numbers", [](int a, int b) -> int { return a + b; });

    auto *fptr = cc.find("sum");
    CHECK((*fptr)(1, 2) == 3);
}
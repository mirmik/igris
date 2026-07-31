#include <doctest/doctest.h>
#include <igris/protocols/gstuff.h>

TEST_CASE("gstuffing")
{
    iovec arr[3] = {
        {(void *)"hello", 5},
        {(void *)"world", 5},
        {(void *)"!", 1},
    };

    auto sbuffer = gstuffing_v(arr, 3, gstuff_context());

    CHECK(sbuffer.size() == 14);
}

TEST_CASE("gstuff vector output covers worst case")
{
    gstuff_context ctx;
    const char data[] = {ctx.GSTUFF_START, ctx.GSTUFF_STUB, ctx.GSTUFF_STOP};
    auto encoded = gstuffing(igris::buffer(data, sizeof(data)), ctx);
    CHECK(encoded.size() <= sizeof(data) * 2 + 4);
    CHECK(encoded.front() == static_cast<uint8_t>(ctx.GSTUFF_START));
    CHECK(encoded.back() == static_cast<uint8_t>(ctx.GSTUFF_STOP));
}

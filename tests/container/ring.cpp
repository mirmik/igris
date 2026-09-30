#include <doctest/doctest.h>
#include <igris/container/ring.h>

class ring_A
{
};

class ring_B
{
    //	int i;

public:
    ring_B() {}
    //	B(int j) : i(j) {}
};

TEST_CASE("ring")
{
    igris::ring<ring_A> aring(12);
    igris::ring<ring_B> bring;

    bring.resize(3);

    CHECK_EQ(aring.avail(), 0);
    CHECK_EQ(aring.room(), 12);

    CHECK_EQ(bring.avail(), 0);
    CHECK_EQ(bring.room(), 3);

    aring.push(ring_A());
    CHECK_EQ(aring.room(), 11);
    aring.pop();
    CHECK_EQ(aring.room(), 12);
}

TEST_CASE("ring.get_last")
{
    igris::ring<int> aring(12);

    aring.push(1);
    aring.push(2);
    aring.push(3);
    aring.push(4);
    aring.push(5);
    aring.push(6);

    CHECK_EQ(aring.last(), 6);

    {
        auto v1 = aring.get_last(0, 3, true);
        CHECK_EQ(v1[0], 6);
        CHECK_EQ(v1[1], 5);
        CHECK_EQ(v1[2], 4);
        CHECK_EQ(v1, std::vector<int>{6, 5, 4});
    }

    {
        auto v2 = aring.get_last(0, 3, false);
        CHECK_EQ(v2[0], 4);
        CHECK_EQ(v2[1], 5);
        CHECK_EQ(v2[2], 6);
        CHECK_EQ(v2, std::vector<int>{4, 5, 6});
    }
}

TEST_CASE("ring container")
{
    igris::ring<std::string> r(6);

    r.push("Lalala");
}

TEST_CASE("ring resize and negative index wrap")
{
    igris::ring<int> r;
    r.resize(3);
    CHECK_EQ(r.room(), 3);
    CHECK_EQ(r.fixup_index(-1), 3);

    r.push(1);
    r.push(2);
    r.push(3);
    r.push(4);
    CHECK_EQ(r.avail(), 3);
    CHECK_EQ(r.tail(), 1);
}

TEST_CASE("ring zero and one element capacities")
{
    igris::ring<int> r;
    CHECK_EQ(r.room(), 0);
    r.push(1);
    CHECK(r.empty());

    r.resize(0);
    CHECK_EQ(r.room(), 0);
    r.resize(1);
    CHECK_EQ(r.room(), 1);
    r.push(7);
    CHECK_EQ(r.last(), 7);
}

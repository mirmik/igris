#include <doctest/doctest.h>
#include <igris/container/ring.h>

class A
{
};

class B
{
    //	int i;

public:
    B() {}
    //	B(int j) : i(j) {}
};

TEST_CASE("ring")
{
    igris::ring<A> aring(12);
    igris::ring<B> bring;

    bring.resize(3);

    CHECK_EQ(aring.avail(), 0);
    CHECK_EQ(aring.room(), 12);

    CHECK_EQ(bring.avail(), 0);
    CHECK_EQ(bring.room(), 3);

    aring.push(A());
    CHECK_EQ(aring.room(), 11);
    aring.pop();
    CHECK_EQ(aring.room(), 12);
}

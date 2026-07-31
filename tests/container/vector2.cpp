#include <doctest/doctest.h>
#include <igris/container/vector2.h>

struct A
{
    int a;
    A() : a(0) {}
    A(int _a) : a(_a) {}
    A(const A &) = default;
};

struct vector2_tracked
{
    static int alive;
    int value = 0;
    vector2_tracked(int value = 0) : value(value) { ++alive; }
    vector2_tracked(const vector2_tracked &oth) : value(oth.value) { ++alive; }
    vector2_tracked(vector2_tracked &&oth) noexcept : value(oth.value) { ++alive; }
    vector2_tracked &operator=(const vector2_tracked &) = default;
    vector2_tracked &operator=(vector2_tracked &&) = default;
    ~vector2_tracked() { --alive; }
};
int vector2_tracked::alive = 0;

TEST_CASE("vector2")
{

    igris::vector2<A> vec;

    vec.push_back(A(33));
    vec.push_back(A(42));
    vec.push_back(A(54));
    vec.push_back(A(78));
    vec.push_back(A(22));

    CHECK_EQ(vec.size(), 5);
    CHECK_EQ(vec[0].a, 33);
    CHECK_EQ(vec[1].a, 42);
    CHECK_EQ(vec[2].a, 54);
    CHECK_EQ(vec[3].a, 78);
    CHECK_EQ(vec[4].a, 22);

    vec.invalidate();
    CHECK_EQ(vec.size(), 0);
}

TEST_CASE("vector2 copy assignment and resize")
{
    igris::vector2<A> source;
    source.push_back(A(1));
    source.push_back(A(2));

    igris::vector2<A> target;
    target.push_back(A(9));
    target = source;
    CHECK_EQ(target.size(), 2);
    CHECK_EQ(target[0].a, 1);
    CHECK_EQ(target[1].a, 2);

    target.resize(1);
    CHECK_EQ(target.size(), 1);
}

TEST_CASE("vector2 manages non-trivial object lifetime")
{
    vector2_tracked::alive = 0;
    {
        igris::vector2<vector2_tracked> source;
        source.push_back(vector2_tracked(1));
        source.push_back(vector2_tracked(2));
        igris::vector2<vector2_tracked> target;
        target.push_back(vector2_tracked(9));
        target = source;
        target.resize(1);
        igris::vector2<vector2_tracked> moved;
        moved = static_cast<igris::vector2<vector2_tracked> &&>(target);
    }
    CHECK_EQ(vector2_tracked::alive, 0);
}

#include "doctest/doctest.h"

#include <compat/std/functional>

static int free_function(int x)
{
    return x * 2;
}

static int free_function_sum(int a, int b)
{
    return a + b;
}

struct Functor
{
    int value;
    Functor(int v) : value(v) {}
    int operator()(int x) const { return x + value; }
};

struct Counter
{
    int count = 0;
    void increment() { ++count; }
    int get() const { return count; }
};

TEST_CASE("igris_std function default constructor")
{
    igris_std::function<int(int)> f;
    CHECK_FALSE(static_cast<bool>(f));
}

TEST_CASE("igris_std function nullptr constructor")
{
    igris_std::function<int(int)> f(nullptr);
    CHECK_FALSE(static_cast<bool>(f));
}

TEST_CASE("igris_std function from free function")
{
    igris_std::function<int(int)> f = free_function;
    CHECK(static_cast<bool>(f));
    CHECK(f(5) == 10);
}

TEST_CASE("igris_std function from lambda")
{
    int captured = 10;
    igris_std::function<int(int)> f = [captured](int x) {
        return x + captured;
    };
    CHECK(static_cast<bool>(f));
    CHECK(f(5) == 15);
}

TEST_CASE("igris_std function from functor")
{
    Functor functor(100);
    igris_std::function<int(int)> f = functor;
    CHECK(static_cast<bool>(f));
    CHECK(f(5) == 105);
}

TEST_CASE("igris_std function copy constructor")
{
    igris_std::function<int(int)> f1 = free_function;
    igris_std::function<int(int)> f2(f1);
    CHECK(static_cast<bool>(f2));
    CHECK(f2(7) == 14);
    CHECK(f1(7) == 14);
}

TEST_CASE("igris_std function copy constructor from empty")
{
    igris_std::function<int(int)> f1;
    igris_std::function<int(int)> f2(f1);
    CHECK_FALSE(static_cast<bool>(f2));
}

TEST_CASE("igris_std function move constructor")
{
    igris_std::function<int(int)> f1 = free_function;
    igris_std::function<int(int)> f2(igris_std::move(f1));
    CHECK(static_cast<bool>(f2));
    CHECK(f2(3) == 6);
    CHECK_FALSE(static_cast<bool>(f1));
}

TEST_CASE("igris_std function copy assignment")
{
    igris_std::function<int(int)> f1 = free_function;
    igris_std::function<int(int)> f2;
    f2 = f1;
    CHECK(static_cast<bool>(f2));
    CHECK(f2(4) == 8);
}

TEST_CASE("igris_std function copy assignment from empty")
{
    igris_std::function<int(int)> f1;
    igris_std::function<int(int)> f2 = free_function;
    f2 = f1;
    CHECK_FALSE(static_cast<bool>(f2));
}

TEST_CASE("igris_std function move assignment")
{
    igris_std::function<int(int)> f1 = free_function;
    igris_std::function<int(int)> f2;
    f2 = igris_std::move(f1);
    CHECK(static_cast<bool>(f2));
    CHECK(f2(6) == 12);
    CHECK_FALSE(static_cast<bool>(f1));
}

TEST_CASE("igris_std function assign nullptr")
{
    igris_std::function<int(int)> f = free_function;
    CHECK(static_cast<bool>(f));
    f = nullptr;
    CHECK_FALSE(static_cast<bool>(f));
}

TEST_CASE("igris_std function assign new callable")
{
    igris_std::function<int(int)> f = free_function;
    CHECK(f(2) == 4);
    f = [](int x) { return x * 3; };
    CHECK(f(2) == 6);
}

TEST_CASE("igris_std function multiple arguments")
{
    igris_std::function<int(int, int)> f = free_function_sum;
    CHECK(f(3, 4) == 7);
}

TEST_CASE("igris_std function void return type")
{
    int result = 0;
    igris_std::function<void(int)> f = [&result](int x) { result = x; };
    f(42);
    CHECK(result == 42);
}

TEST_CASE("igris_std function swap")
{
    igris_std::function<int(int)> f1 = [](int x) { return x * 2; };
    igris_std::function<int(int)> f2 = [](int x) { return x * 3; };
    f1.swap(f2);
    CHECK(f1(5) == 15);
    CHECK(f2(5) == 10);
}

TEST_CASE("igris_std function free swap")
{
    igris_std::function<int(int)> f1 = [](int x) { return x + 1; };
    igris_std::function<int(int)> f2 = [](int x) { return x + 2; };
    igris_std::swap(f1, f2);
    CHECK(f1(10) == 12);
    CHECK(f2(10) == 11);
}

TEST_CASE("igris_std function self assignment")
{
    igris_std::function<int(int)> f = free_function;
    f = f;
    CHECK(static_cast<bool>(f));
    CHECK(f(5) == 10);
}

TEST_CASE("igris_std function with mutable lambda")
{
    int counter = 0;
    igris_std::function<int()> f = [counter]() mutable {
        return ++counter;
    };
    CHECK(f() == 1);
    CHECK(f() == 2);
    CHECK(f() == 3);
}

TEST_CASE("igris_std function result_type")
{
    using func_type = igris_std::function<int(double)>;
    CHECK((igris_std::is_same<func_type::result_type, int>::value));
}

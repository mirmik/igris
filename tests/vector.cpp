#include <doctest/doctest.h>
#include <igris/container/vector.h>

namespace
{
    struct vector_tracked
    {
        static int alive;
        int value = 0;
        vector_tracked(int value = 0) : value(value) { ++alive; }
        vector_tracked(const vector_tracked &oth) : value(oth.value) { ++alive; }
        vector_tracked(vector_tracked &&oth) noexcept : value(oth.value) { ++alive; }
        vector_tracked &operator=(const vector_tracked &) = default;
        vector_tracked &operator=(vector_tracked &&) = default;
        ~vector_tracked() { --alive; }
    };
    int vector_tracked::alive = 0;
}

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

TEST_CASE("vector.pop_back")
{
    igris::vector<double> vec;

    vec.push_back(15.5);
    vec.push_back(15.25);
    vec.push_back(3);
    vec.push_back(2);

    vec.pop_back();
    vec.pop_back();
    vec.pop_back();
    vec.pop_back();

    CHECK_EQ(vec.size(), 0);
}

TEST_CASE("vector.erase")
{
    igris::vector<double> vec;

    vec.push_back(15.5);
    vec.push_back(15.25);
    vec.push_back(3);
    vec.push_back(2);

    vec.erase(vec.begin() + 1);
    vec.erase(vec.begin() + 1);
    vec.erase(vec.begin() + 1);
    vec.erase(vec.begin());

    CHECK_EQ(vec.size(), 0);
}

TEST_CASE("vector copy assignment and object lifetime")
{
    vector_tracked::alive = 0;
    {
        igris::vector<vector_tracked> source;
        source.emplace_back(1);
        source.emplace_back(2);
        source.emplace_back(3);

        igris::vector<vector_tracked> target;
        target.emplace_back(9);
        target = source;
        CHECK_EQ(target.size(), 3);
        CHECK_EQ(target[1].value, 2);

        target.insert(target.begin() + 1, vector_tracked(7));
        target.erase(target.begin() + 2, target.end());
        CHECK_EQ(target.size(), 2);
    }
    CHECK_EQ(vector_tracked::alive, 0);
}

TEST_CASE("vector.insert")
{
    igris::vector<double> vec;

    vec.push_back(15.5);
    vec.push_back(15.25);
    vec.push_back(3);
    vec.push_back(2);

    vec.insert(vec.begin() + 1, 1);
    vec.insert(vec.begin() + 1, 2);
    vec.insert(vec.begin() + 1, 3);
    vec.insert(vec.begin() + 1, 4);

    CHECK_EQ(vec[0], 15.5);
    CHECK_EQ(vec[1], 4);
    CHECK_EQ(vec[2], 3);
    CHECK_EQ(vec[3], 2);
    CHECK_EQ(vec[4], 1);
    CHECK_EQ(vec[5], 15.25);
    CHECK_EQ(vec[6], 3);
    CHECK_EQ(vec[7], 2);
}

TEST_CASE("vector inserts into an empty container")
{
    igris::vector<int> vec;
    vec.insert(vec.begin(), 42);
    CHECK_EQ(vec.size(), 1);
    CHECK_EQ(vec.front(), 42);
}

TEST_CASE("vector.reserve")
{
    igris::vector<double> vec;

    vec.reserve(1000);
    vec.push_back(15.5);
    vec.push_back(15.25);
    vec.push_back(3);
    vec.push_back(2);

    CHECK_EQ(vec[0], 15.5);
    CHECK_EQ(vec.capacity(), 1000);
    CHECK_EQ(vec.size(), 4);
}

TEST_CASE("vector.erase")
{
    igris::vector<double> vec{1, 2, 3, 4, 5, 6, 7, 8, 9};
    vec.erase(std::remove_if(
                  vec.begin(), vec.end(), [](int i) { return i % 2 == 0; }),
              vec.end());

    CHECK_EQ(vec[0], 1);
    CHECK_EQ(vec[1], 3);
    CHECK_EQ(vec[2], 5);
    CHECK_EQ(vec[3], 7);
    CHECK_EQ(vec[4], 9);
}

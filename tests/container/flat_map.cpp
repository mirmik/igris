#include <doctest/doctest.h>
#include <igris/container/flat_map.h>

#include <string>

TEST_CASE("flat_map")
{
    igris::flat_map<std::string, int> map;

    map["a"] = 1;
    map["b"] = 2;
    map["c"] = 3;
    map["d"] = 4;

    SUBCASE("find")
    {
        REQUIRE(map.find("a")->second == 1);
        REQUIRE(map.find("b")->second == 2);
        REQUIRE(map.find("c")->second == 3);
        REQUIRE(map.find("d")->second == 4);
    }

    SUBCASE("at")
    {
        REQUIRE(map.at("a") == 1);
        REQUIRE(map.at("b") == 2);
        REQUIRE(map.at("c") == 3);
        REQUIRE(map.at("d") == 4);
    }

    REQUIRE_THROWS_AS(map.at("e"), std::out_of_range);
    REQUIRE(map.find("e") == map.end());
    REQUIRE(map.size() == 4);
}

TEST_CASE("flat_map keeps keys ordered and const lookup is safe")
{
    igris::flat_map<int, int> map{{3, 30}, {1, 10}, {2, 20}, {2, 99}};
    map[0] = 0;
    map.emplace(4, 40);

    int expected = 0;
    for (const auto &entry : map)
        CHECK_EQ(entry.first, expected++);

    const auto &const_map = map;
    CHECK_EQ(const_map[3], 30);
    CHECK_THROWS_AS(const_map[8], std::out_of_range);
}

TEST_CASE("flat_map honors custom comparator")
{
    igris::flat_map<int, int, std::greater<int>> map{{1, 10}, {3, 30}};
    map.emplace(2, 20);
    map.insert({4, 40});
    int expected = 4;
    for (const auto &entry : map)
        CHECK_EQ(entry.first, expected--);
}

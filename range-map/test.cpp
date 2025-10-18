#include "range_map.h"
#include "utils.h"

#include <string>
#include <limits>
#include <map>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Basic") {
    RangeMap<int, int> map;

    map.Insert({1, 2}, 1);
    REQUIRE_FALSE(map.Empty());
    REQUIRE(map.Size() == 1uz);
    REQUIRE(map.Contains(1));
    REQUIRE_FALSE(map.Contains(2));
    REQUIRE(map.Get(1) == 1);
    REQUIRE_THROWS(map.Get(2));

    map.Insert({2, 3}, 2);
    map.Insert({3, 4}, 3);
    REQUIRE(map.Size() == 3uz);
    REQUIRE(map.Get(1) == 1);
    REQUIRE(map.Get(2) == 2);
    REQUIRE(map.Get(3) == 3);
    REQUIRE_THROWS(map.Get(4));

    map.Remove({1, 2});
    REQUIRE(map.Size() == 2uz);
    REQUIRE_THROWS(map.Get(1));
    REQUIRE(map.Get(2) == 2);
    REQUIRE(map.Get(3) == 3);

    map.Clear();
    REQUIRE(map.Empty());
    REQUIRE(map.Size() == 0uz);
    REQUIRE_THROWS(map.Get(2));
    REQUIRE_THROWS(map.Get(3));
}

TEST_CASE("Constness") {
    RangeMap<int, int> map;

    map.Insert({1, 2}, 1);

    const auto& rmap = map;
    REQUIRE(rmap.Size() == 1uz);
    REQUIRE_FALSE(rmap.Empty());
    REQUIRE(rmap.Get(1) == 1);
    REQUIRE_FALSE(rmap.Contains(2));
}

TEST_CASE("Constructor") {
    RangeMap<int, int> map({{{1, 2}, 1}, {{2, 3}, 2}, {{3, 4}, 3}});
    REQUIRE_FALSE(map.Empty());
    REQUIRE(map.Size() == 3uz);
    REQUIRE(map.Get(1) == 1);
    REQUIRE(map.Get(2) == 2);
    REQUIRE(map.Get(3) == 3);
    REQUIRE_THROWS(map.Get(4));

    map.Insert({4, 5}, 4);
    REQUIRE(map.Size() == 4uz);
    REQUIRE(map.Get(1) == 1);
    REQUIRE(map.Get(2) == 2);
    REQUIRE(map.Get(3) == 3);
    REQUIRE(map.Get(4) == 4);
    REQUIRE_THROWS(map.Get(5));

    map.Insert({1, 2}, 10);
    REQUIRE(map.Size() == 4uz);
    REQUIRE(map.Get(1) == 10);
    REQUIRE(map.Get(2) == 2);
    REQUIRE(map.Get(3) == 3);
    REQUIRE(map.Get(4) == 4);
}

TEST_CASE("Basic Mixed Types") {
    RangeMap<int, std::string> map;
    REQUIRE(map.Empty());

    map.Insert({1, 2}, "a");
    REQUIRE_FALSE(map.Empty());
    REQUIRE(map.Size() == 1uz);
    REQUIRE(map.Contains(1));
    REQUIRE(map.Get(1) == "a");

    map.Insert({2, 3}, "b");
    map.Insert({3, 4}, "c");
    REQUIRE(map.Size() == 3uz);
    REQUIRE(map.Get(1) == "a");
    REQUIRE(map.Get(2) == "b");
    REQUIRE(map.Get(3) == "c");

    map.Remove({1, 2});
    REQUIRE_THROWS(map.Get(1));
    REQUIRE(map.Size() == 2uz);
    REQUIRE(map.Get(2) == "b");
    REQUIRE(map.Get(3) == "c");

    map.Insert({2, 3}, "bb");
    REQUIRE(map.Size() == 2uz);
    REQUIRE(map.Get(2) == "bb");
    REQUIRE(map.Get(3) == "c");
}

TEST_CASE("Huge") {
    RangeMap<int, int> map;
    map.Insert({std::numeric_limits<int>::min(), std::numeric_limits<int>::max()}, 1);
    RandomGenerator gen{42uz};
    REQUIRE(map.Get(gen.GenInt<int>()) == 1);
}

TEST_CASE("Not Valid") {
    RangeMap<int, int> map;
    REQUIRE_THROWS(map.Insert({1, 1}, 1));
    REQUIRE_THROWS(map.Insert({3, 1}, 1));
}

TEST_CASE("Strings") {
    RangeMap<std::string, int> map;
    map.Insert({"123", "129"}, 1);
    REQUIRE(map.Get("128") == 1);
    map.Insert({"12300", "12400"}, 2);
    REQUIRE(map.Get("12350") == 2);
    map.Insert({"123", "12350"}, 3);
    REQUIRE(map.Get("123") == 3);
    REQUIRE(map.Get("1230") == 3);
    REQUIRE(map.Get("12340") == 3);
}

TEST_CASE("Overlaps") {
    RangeMap<int, int> map({{{1, 2}, 1}, {{2, 3}, 2}, {{3, 4}, 3}});
    map.Insert({1, 3}, 4);
    REQUIRE(map.Size() == 2uz);
    REQUIRE(map.Get(1) == 4);
    REQUIRE(map.Get(2) == 4);

    map.Insert({3, 10}, 5);
    REQUIRE(map.Size() == 2uz);
    REQUIRE(map.Get(3) == 5);
    REQUIRE(map.Get(5) == 5);

    map.Insert({1, 10}, 10);
    REQUIRE(map.Size() == 1uz);
    for (int i = 1; i < 10; ++i) {
        CHECK(map.Get(i) == 10);
    }
}

TEST_CASE("Touch") {
    RangeMap<int, int> map;

    map.Insert({1, 5}, 100);
    map.Insert({5, 10}, 200);
    REQUIRE(map.Size() == 2uz);
    for (int i = 1; i < 10; ++i) {
        CHECK(map.Get(i) == (i < 5 ? 100 : 200));
    }

    map.Remove({2, 3});
    REQUIRE(map.Size() == 3uz);
    REQUIRE(map.Get(1) == 100);
    REQUIRE(map.Get(4) == 100);
    REQUIRE(map.Get(6) == 200);
    REQUIRE_THROWS(map.Get(2));

    map.Insert({3, 7}, 300);
    REQUIRE(map.Size() == 3uz);
    for (int i = 1; i < 2; ++i) {
        CHECK(map.Get(i) == 100);
    }
    for (int i = 3; i < 7; ++i) {
        CHECK(map.Get(i) == 300);
    }
    for (int i = 7; i < 10; ++i) {
        CHECK(map.Get(i) == 200);
    }
}

TEST_CASE("Freestyle") {
    RangeMap<int, int> map;
    map.Insert({1, 4}, 100);
    map.Insert({4, 7}, 200);
    map.Insert({7, 10}, 300);

    REQUIRE(map.Size() == 3uz);
    REQUIRE(map.Get(1) == 100);
    REQUIRE(map.Get(4) == 200);
    REQUIRE(map.Get(7) == 300);

    map.Insert({3, 5}, 400);
    REQUIRE(map.Size() == 4uz);
    REQUIRE(map.Get(2) == 100);
    REQUIRE(map.Get(3) == 400);
    REQUIRE(map.Get(4) == 400);
    REQUIRE(map.Get(5) == 200);

    map.Insert({6, 11}, 500);
    REQUIRE(map.Size() == 4uz);
    REQUIRE(map.Get(5) == 200);
    REQUIRE(map.Get(7) == 500);
    REQUIRE(map.Get(10) == 500);

    map.Insert({-1, 100}, 0);
    REQUIRE(map.Size() == 1uz);
}

TEST_CASE("Range map can be simple map") {
    std::map<int, int> map;
    RangeMap<int, int> range;

    std::mt19937_64 gen{43};
    UniformIntDistribution dist{0, 200'000};

    for (auto i = 0; i < 3'000'000; ++i) {
        auto key = dist(gen);

        if (auto command = dist(gen) % 3; command == 0) {
            auto value = dist(gen);
            auto [it, ok] = map.emplace(key, value);

            if (ok) {
                CHECK_FALSE(range.Contains(key));
                range.Insert({key, key + 1}, value);
            } else {
                CHECK(range.Get(key) == it->second);
            }
        } else if (command == 1) {
            CHECK(range.Contains(key) == map.contains(key));
        } else if (map.erase(key)) {
            CHECK(range.Contains(key));
            range.Remove({key, key + 1});
        } else {
            CHECK_FALSE(range.Contains(key));
        }

        CHECK(range.Size() == map.size());
    }
}

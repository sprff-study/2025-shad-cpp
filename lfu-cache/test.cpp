#include "lfu_cache.h"
#include "dist.h"

#include <string>
#include <ranges>
#include <vector>
#include <algorithm>
#include <concepts>
#include <random>
#include <map>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Check class") {
    using CacheType = LFUCache<std::string, std::string>;
    STATIC_CHECK_FALSE(std::copy_constructible<CacheType>);
    STATIC_CHECK_FALSE(std::move_constructible<CacheType>);
    STATIC_CHECK_FALSE(std::assignable_from<CacheType, const CacheType&>);
    STATIC_CHECK_FALSE(std::assignable_from<CacheType, CacheType&&>);
}

TEST_CASE("Set and get") {
    using CacheType = LFUCache<std::string, int>;
    CacheType cache(10);

    cache.Set("a", 1);
    cache.Set("b", 2);
    cache.Set("c", 3);

    REQUIRE(cache.Get("a") == 1);
    REQUIRE(cache.Get("b") == 2);
    REQUIRE(cache.Get("c") == 3);

    REQUIRE_THROWS(cache.Get("d"));

    cache.Set("c", 4);
    REQUIRE(cache.Get("c") == 4);
}

TEST_CASE("Erase") {
    using CacheType = LFUCache<std::string, int>;
    CacheType cache(10);

    cache.Set("a", 1);
    cache.Set("b", 2);
    cache.Set("c", 3);

    REQUIRE(cache.Get("a") == 1);
    REQUIRE(cache.Get("b") == 2);
    REQUIRE(cache.Get("c") == 3);
    REQUIRE_FALSE(cache.Erase("d"));

    REQUIRE(cache.Erase("a"));
    REQUIRE_THROWS(cache.Get("a"));
    REQUIRE(cache.Get("b") == 2);
    REQUIRE(cache.Get("c") == 3);

    REQUIRE(cache.Erase("c"));
    REQUIRE(cache.Erase("b"));
    REQUIRE_THROWS(cache.Get("b"));
    REQUIRE_THROWS(cache.Get("c"));
}

TEST_CASE("Eviction") {
    using CacheType = LFUCache<std::string, int>;
    CacheType cache(2);

    cache.Set("a", 1);
    cache.Set("b", 2);
    cache.Set("c", 3);

    REQUIRE_THROWS(cache.Get("a"));
    REQUIRE(cache.Get("b") == 2);
    REQUIRE(cache.Get("c") == 3);

    cache.Set("b", 4);
    cache.Set("c", 5);
    cache.Set("b", 6);

    cache.Set("e", 7);
    REQUIRE_THROWS(cache.Get("c"));
    REQUIRE(cache.Get("b") == 6);
    REQUIRE(cache.Get("e") == 7);

    cache.Get("b");
    cache.Set("f", 8);
    REQUIRE_THROWS(cache.Get("e"));
    REQUIRE(cache.Get("b") == 6);
    REQUIRE(cache.Get("f") == 8);
}

TEST_CASE("Frequency First") {
    using CacheType = LFUCache<std::string, int>;
    CacheType cache(2);

    cache.Set("a", 1);
    REQUIRE(cache.Get("a") == 1);
    cache.Set("b", 2);
    cache.Set("c", 3);

    REQUIRE_THROWS(cache.Get("b"));
    REQUIRE(cache.Get("a") == 1);
    REQUIRE(cache.Get("c") == 3);

    cache.Set("b", 4);
    REQUIRE_THROWS(cache.Get("c"));
    cache.Set("c", 5);
    REQUIRE_THROWS(cache.Get("b"));
    cache.Set("b", 6);
    REQUIRE_THROWS(cache.Get("c"));

    cache.Set("e", 7);
    REQUIRE_THROWS(cache.Get("c"));
    REQUIRE_THROWS(cache.Get("b"));
    REQUIRE(cache.Get("a") == 1);
    REQUIRE(cache.Get("e") == 7);

    cache.Get("e");
    cache.Set("e", 8);
    cache.Set("f", 9);
    REQUIRE_THROWS(cache.Get("a"));
    REQUIRE(cache.Get("e") == 8);
    REQUIRE(cache.Get("f") == 9);
}

TEST_CASE("Frequency Second") {
    using CacheType = LFUCache<std::string, int>;
    CacheType cache(3);

    cache.Set("a", 1);
    cache.Set("b", 2);
    cache.Set("c", 3);
    for (auto _ = 0; _ < 10; ++_) {
        cache.Get("a");
    }
    cache.Get("b");
    cache.Get("c");
    cache.Set("d", 4);
    REQUIRE(cache.Get("a") == 1);
    REQUIRE(cache.Get("d") == 4);
}

TEST_CASE("Stress 1") {
    using CacheType = LFUCache<int, int>;
    constexpr auto kSize = 1'000;
    constexpr auto kEnd = 100 * kSize;

    CacheType cache(kSize);
    for (auto i : std::views::iota(0, kSize)) {
        cache.Set(i, i);
        CHECK(cache.Get(i) == i);
    }

    for (auto i : std::views::iota(kSize, kEnd)) {
        cache.Set(i, kSize + 1);
        CHECK(cache.Get(i) == kSize + 1);
        auto old_key = i - kSize;
        CHECK_THROWS(cache.Get(old_key));
    }
}

TEST_CASE("Stress 2") {
    using CacheType = LFUCache<int, int>;
    constexpr auto kSize = 100'000;
    constexpr auto kRange = std::views::iota(0, kSize);

    std::mt19937 gen{31'134};
    auto v = std::ranges::to<std::vector>(kRange);
    std::ranges::shuffle(v, gen);

    CacheType cache(kSize);
    for (auto i : v) {
        cache.Set(i, 42);
    }

    std::ranges::shuffle(v, gen);
    for (auto i : v) {
        CHECK(cache.Get(i) == 42);
    }

    for (auto i : kRange) {
        cache.Set(i + kSize, 0);
        CHECK(cache.Get(i + kSize) == 0);
        CHECK_THROWS(cache.Get(v[i]));
    }

    for (auto i : std::views::iota(kSize, 2 * kSize)) {
        CHECK(cache.Get(i) == 0);
    }
}

TEST_CASE("Stress 3") {
    constexpr auto kSize = 10'000;

    std::map<int, int> map;
    LFUCache<int, int> cache{kSize};

    std::mt19937_64 gen{43};
    UniformIntDistribution dist{1, kSize};

    for (auto i = 0; i < 100'000; ++i) {
        auto key = dist(gen);

        if (auto command = dist(gen) % 4; command < 2) {
            auto value = dist(gen);
            auto [it, ok] = map.emplace(key, value);

            if (ok) {
                CHECK_THROWS(cache.Get(key));
            } else {
                CHECK_NOTHROW(cache.Get(key));
                it->second = value;
            }

            cache.Set(key, value);
            CHECK(cache.Get(key) == it->second);
        } else if (command < 3) {
            if (auto it = map.find(key); it != map.end()) {
                CHECK(cache.Get(key) == it->second);
            } else {
                CHECK_THROWS(cache.Get(key));
            }
        } else if (map.erase(key)) {
            CHECK_NOTHROW(cache.Get(key));
            CHECK(cache.Erase(key));
        } else {
            CHECK_FALSE(cache.Erase(key));
            CHECK_THROWS(cache.Get(key));
        }
    }
}

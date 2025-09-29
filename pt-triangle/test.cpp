#include "point_triangle.h"

#include <catch2/catch_test_macros.hpp>

namespace {

auto test = 0;

void Check(bool result) {
    INFO("Test " << ++test);
    CHECK(result);
}

void CheckFalse(bool result) {
    INFO("Test " << ++test);
    CHECK_FALSE(result);
}

}  // namespace

TEST_CASE("Simple") {
    Check(IsPointInTriangle({{-2, 0}, {2, 0}, {0, 2}}, {0, 1}));
    Check(IsPointInTriangle({{-4, -5}, {0, 7}, {3, 1}}, {0, 0}));
    Check(IsPointInTriangle({{-1, 1}, {-1, 0}, {1000, 1}}, {-1, 0}));
    Check(IsPointInTriangle({{0, 0}, {-10, 10}, {1, 11}}, {-5, 5}));
    CheckFalse(IsPointInTriangle({{-19, 5}, {5, 19}, {4, -7}}, {4, -8}));
    CheckFalse(IsPointInTriangle({{-2, 2}, {-2, 3}, {-3, 3}}, {-3, 2}));
}

TEST_CASE("ZeroSize") {
    CheckFalse(IsPointInTriangle({{0, 0}, {0, 10}, {10, 0}}, {11, 0}));
}

TEST_CASE("Big") {
    constexpr auto kMax = 1'000'000'000;
    Check(IsPointInTriangle({{-kMax, -kMax}, {kMax, kMax}, {kMax, -kMax}}, {-kMax + 1, -kMax + 1}));
    CheckFalse(IsPointInTriangle({{-kMax, kMax}, {-kMax, -kMax}, {kMax, kMax}}, {kMax, kMax - 1}));
    Check(IsPointInTriangle({{-kMax, -1}, {kMax, 2}, {-1, kMax - 1}}, {-2, kMax - 10}));
    Check(
        IsPointInTriangle({{-kMax, -kMax}, {-kMax + 2, -kMax}, {kMax, 0}}, {-kMax + 3, -kMax + 1}));
    CheckFalse(IsPointInTriangle({{0, kMax}, {-1, -kMax}, {1, -kMax}}, {1, kMax - 1}));
    Check(IsPointInTriangle({{-1, -kMax}, {1, -kMax}, {0, kMax}}, {0, kMax - 1}));
}

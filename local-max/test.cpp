#include "local_max.h"
#include "strict_iterator.h"

#include <vector>
#include <list>
#include <initializer_list>
#include <ranges>
#include <cmath>

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>

namespace {

class Int {
public:
    explicit Int(int x) : x_{x} {
    }

    Int(const Int&) = delete;
    Int(Int&&) = delete;
    Int& operator=(const Int&) = delete;
    Int& operator=(Int&&) = delete;

    bool operator<(const Int& other) const {
        return x_ < other.x_;
    }

private:
    int x_;
};

template <class Iterator>
void Check(Iterator first, Iterator last, Iterator expected) {
    auto first_strict = MakeStrict(first, first, last);
    auto last_strict = MakeStrict(first, last, last);
    auto it = LocalMax(first_strict, last_strict).Base();
    CHECK(it == expected);
}

void Check(const std::vector<int>& data, size_t index) {
    Check(data.begin(), data.end(), data.begin() + index);
}

void CheckInts(std::initializer_list<int> init_list, size_t index) {
    std::list<Int> list;
    auto expected = list.end();

    for (size_t i = 0; auto x : init_list) {
        list.emplace_back(x);
        if (i++ == index) {
            expected = --list.end();
        }
    }

    Check(list.begin(), list.end(), expected);
}

}  // namespace

TEST_CASE("Simple") {
    std::vector<int> data = {1, 2, 3, 2, 1};

    CHECK(LocalMax(data.begin(), data.end()) == data.begin() + 2);
    Check({1, 2, 3, 4, 4, 3, 5, 4}, 6);
}

TEST_CASE("Empty") {
    Check({}, 0);
}

TEST_CASE("Small") {
    Check({1}, 0);
    Check({1, 1}, 2);
    Check({1, 2}, 1);
    Check({2, 1}, 0);
}

TEST_CASE("Tricky") {
    CheckInts({1, 2, 1}, 1);
    CheckInts({2, 1, 1}, 0);
    CheckInts({0, 1, 2}, 2);
    CheckInts({1, 1, 1}, 3);
    CheckInts({-1, 4, 4}, 3);
    CheckInts({8, 8, 7}, 3);
}

TEST_CASE("Big") {
    constexpr auto kSize = 100'000;

    auto index = GENERATE(take(10, random(0, kSize - 1)));
    std::vector<int> data(kSize);
    data[index] = 1;

    Check(data.begin(), data.end(), data.begin() + index);
}

TEST_CASE("Ranges") {
    constexpr auto kRange = std::views::iota(0ll, 1'000'000'000'000'000ll);
    constexpr auto kReversed = std::views::reverse(kRange);

    Check(kReversed.begin(), kReversed.end(), kReversed.begin());

    auto shift = GENERATE(take(10, random(0, 100'000)));
    auto tf_func = [shift](auto x) { return -std::abs(x - shift); };
    auto tf = kRange | std::views::transform(tf_func);

    Check(tf.begin(), tf.end(), tf.begin() + shift);
}

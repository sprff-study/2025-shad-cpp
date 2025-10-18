#include "subsequence.h"

#include <iterator>
#include <vector>
#include <forward_list>
#include <ranges>
#include <list>
#include <functional>

#include <catch2/catch_test_macros.hpp>

namespace {

void Check(const auto& range, auto pred, size_t from, size_t size) {
    auto begin = std::ranges::begin(range);
    auto end = std::ranges::end(range);

    auto expected_first = std::next(begin, from);
    auto expected_last = std::next(expected_first, size);

    auto [first, last] = GetLongestSubsequence(begin, end, pred);

    CHECK(first == expected_first);
    CHECK(last == expected_last);
}

using ForwardList = std::forward_list<int>;

}  // namespace

TEST_CASE("Empty") {
    Check(std::vector<int>{}, [](int, int) { return false; }, 0, 0);
    Check(std::forward_list<int>{}, [](int, int) { return false; }, 0, 0);
}

TEST_CASE("Simple") {
    using FL = std::forward_list<int>;

    std::equal_to pred{};

    Check(FL{1}, pred, 1, 0);
    Check(FL{1, 2}, pred, 2, 0);
    Check(FL{1, 2, 3}, pred, 3, 0);
    Check(FL{2, 1, 2}, pred, 3, 0);
    Check(FL{2, 1, 2, 1}, pred, 4, 0);

    Check(FL{1, 1}, pred, 0, 2);
    Check(FL{1, 1, 2}, pred, 0, 2);
    Check(FL{2, 1, 1}, pred, 1, 2);
    Check(FL{2, 2, 2}, pred, 0, 3);
}

TEST_CASE("Multiple") {
    using FL = std::forward_list<int>;

    std::equal_to pred{};

    Check(FL{1, 4, 2, 2, 3, 2, 2}, pred, 2, 2);
    Check(FL{1, 1, 2, 1, 1, 1}, pred, 3, 3);
}

TEST_CASE("All") {
    std::vector<int> v(100'000);
    Check(v, std::equal_to{}, 0, v.size());

    auto range = std::views::iota(0, 5'000);
    Check(range, std::less{}, 0, range.size());
}

TEST_CASE("None") {
    std::vector<int> v(100'000);
    Check(v, std::not_equal_to{}, v.size(), 0);

    auto range = std::views::iota(0, 5'000);
    Check(range, std::greater{}, range.size(), 0);
}

TEST_CASE("Strings") {
    std::vector<std::string> v = {"aba", "caba", "aba", "daba", "abacaba"};

    auto shorter = [](const std::string& a, const std::string& b) { return a.size() < b.size(); };

    Check(v, shorter, 2, 3);
}

TEST_CASE("Complexity") {
    std::list<int> list;

    for (auto i : std::views::iota(1, 3'001)) {
        list.push_back(1);
        for (auto _ : std::views::iota(0, i)) {
            list.push_back(2);
        }
    }

    Check(list, std::equal_to{}, list.size() - 3'000, 3'000);
    Check(list, std::less_equal{}, list.size() - 3'001, 3'001);

    auto reversed = std::views::reverse(list);

    Check(reversed, std::equal_to{}, 0, 3'000);
    Check(reversed, std::greater_equal{}, 0, 3'001);
}

TEST_CASE("No copy") {
    struct Empty {
        Empty() = default;
        Empty(const Empty&) = delete;
        Empty(Empty&&) = delete;
        Empty& operator=(const Empty&) = delete;
        Empty& operator=(Empty&&) = delete;
    };

    constexpr auto kSize = 1'000;

    std::vector<Empty> empties(kSize);
    auto compare = [](const Empty& a, const Empty& b) { return &a < &b; };

    Check(empties, compare, 0, kSize);
    Check(std::views::reverse(empties), compare, kSize, 0);
}

#pragma once

#include <utility>

template <class It, class Pred>
std::pair<It, It> GetLongestSubsequence(It first, It last, Pred pred) {
    return std::make_pair(last, last);
}

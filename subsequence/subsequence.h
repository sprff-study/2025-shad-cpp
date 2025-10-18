#pragma once

#include <utility>
#include <cstddef>

template <class It, class Pred>
std::pair<It, It> GetLongestSubsequence(It first, It last, Pred pred) {
    size_t anslen = 0;
    It ansl = last;
    It ansr = last;
    It curl = first;
    It curr = first;
    size_t curlen = 1;
    while (curr != last) {
        It nxt = curr;
        ++nxt;
        if (nxt != last && pred(*curr, *nxt)) {
            curr++;
            curlen++;
            if (curlen > anslen) {
                anslen = curlen;
                ansl = curl;
                ansr = curr;
                ++ansr;
            }
        } else {
            curl = nxt;
            curr = nxt;
            curlen = 1;
        }
    }
    return std::make_pair(ansl, ansr);
}

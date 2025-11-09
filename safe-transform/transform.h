#pragma once

#include <iostream>
#include <unordered_map>

template <typename Iter, typename Pred, typename Func>
void TransformIf(Iter begin, Iter end, Pred p, Func f) {
    using T = Iter::value_type;
    std::unordered_map<int, T> log;

    int i = 0;

    try {
        i = 0;
        for (auto it = begin; it != end; ++it, ++i) {
            if (p(*it)) {
                // try {
                log.emplace(i, *it);
                // } catch (...) {
                // }
                f(*it);
            }
        }
    } catch (...) {
        int maxi = i;
        i = 0;
        for (auto it = begin; it != end; ++it, ++i) {
            if (i > maxi) {
                break;
            }
            if (log.contains(i)) {
                *it = log.at(i);
            }
        }
        throw;
    }
}

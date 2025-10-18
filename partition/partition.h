#pragma once

#include <stdexcept>

template <class Iterator, class Predicate>
Iterator Partition(Iterator first, Iterator last, Predicate pred) {
    if (first == last) {
        return last;
    }
    Iterator sep = last;  // first false
    for (auto it = first; it != last; it++) {
        if (pred(*it)) {
            if (sep != last) {
                std::swap(*sep, *it);
                sep++;
            }
        } else {
            if (sep == last) {
                sep = it;
            }
        }
    }
    return sep;
}

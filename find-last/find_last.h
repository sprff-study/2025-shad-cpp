#pragma once

#include <stdexcept>

template <class Iterator, class T>
Iterator FindLast(Iterator first, Iterator last, const T& value) {
    Iterator back = last;
    while (first != back) {
        --back;
        if (*back == value) {
            return back;
        }
    }
    return last;
}

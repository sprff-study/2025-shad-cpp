#pragma once
#include <cstddef>

struct Sum {
    template <typename T>
    T operator()(T a, T b) {
        return a + b;
    }
};

struct Prod {
    template <typename T>
    T operator()(T a, T b) {
        return a * b;
    }
};

struct Concat {
    template <typename T>
    T operator()(T a, T b) {
        for (auto el : b) {
            a.push_back(el);
        }
        return a;
    }
};

class Length {
public:
    Length(size_t *cnt) : cnt_(cnt) {
    }
    template <typename T>
    T operator()(T a, T _) {
        (*cnt_)++;
        return a;
    }

private:
    size_t *cnt_;
};

template <class Iterator, class T, class BinaryOp>
T Fold(Iterator first, Iterator last, T init, BinaryOp func) {
    T res = init;
    while (first != last) {
        res = func(res, *first);
        ++first;
    }
    return res;
}

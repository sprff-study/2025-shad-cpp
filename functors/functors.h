#pragma once

#include <cstddef>
#include <algorithm>

template <class Functor>
class ReverseUnaryFunctor {
public:
    ReverseUnaryFunctor(Functor f) : f_(f) {
    }
    bool operator()(auto v) {
        return !f_(v);
    }

private:
    Functor f_;
};

template <class Functor>
class ReverseBinaryFunctor {
public:
    ReverseBinaryFunctor(Functor f) : f_(f) {
    }
    bool operator()(auto a, auto b) {
        return f_(b, a);
    }

private:
    Functor f_;
};

auto MakeReverseUnaryFunctor(auto functor) {
    return ReverseUnaryFunctor{functor};
}

auto MakeReverseBinaryFunctor(auto functor) {
    return ReverseBinaryFunctor{functor};
}

template <class Iterator>
size_t ComparisonsCount(Iterator first, Iterator last) {
    size_t cnt;
    auto cmp = [&cnt](Iterator a, Iterator b) {
        ++cnt;
        return *a < *b;
    };
    std::sort(first, last, cmp);
}

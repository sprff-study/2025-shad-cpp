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
    size_t cnt = 0;
    auto cmp = [&cnt](const auto &a, const auto &b) {
        ++cnt;
        return a < b;
    };
    std::sort(first, last, cmp);
    return cnt;
}

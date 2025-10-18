#pragma once

#include <cstddef>
#include <utility>
typedef long long ll;

template <class Iterator>
class Sequence {
public:
    Sequence(Iterator begin, Iterator end) : begin_{begin}, end_{end} {
    }

    Iterator begin() const {
        return begin_;
    }

    Iterator end() const {
        return end_;
    }

private:
    Iterator begin_, end_;
};

template <class Iterator>
class RepeatIterator {
public:
    RepeatIterator(Iterator begin, Iterator end, size_t count = 0)
        : begin_{begin}, it_{begin}, end_{end}, count_{count} {
    }

    RepeatIterator& operator++() {
        if (!(++it_ != end_)) {
            ++count_;
            it_ = begin_;
        }
        return *this;
    }

    auto operator*() const {
        return *it_;
    }

    bool operator==(const RepeatIterator& rhs) const = default;

private:
    Iterator begin_, it_, end_;
    size_t count_;
};

auto Repeat(const auto& sequence, size_t n) {
    auto begin = sequence.begin();
    auto end = sequence.end();
    if (n && (begin != end)) {
        return Sequence{RepeatIterator{begin, end}, RepeatIterator{begin, end, n}};
    } else {
        return Sequence{RepeatIterator{end, end}, RepeatIterator{end, end}};
    }
}

/// Range
class RangeIterator {
public:
    RangeIterator(ll cur, ll step) : cur_(cur), step_(step) {
    }
    RangeIterator& operator++() {
        cur_ += step_;
        return *this;
    }
    auto operator*() const {
        return cur_;
    }
    bool operator==(const RangeIterator& rhs) const = default;

private:
    ll cur_;
    ll step_;
};

auto Range(ll from, ll to, ll step = 1) {
    // Assume step > 0; from <= to;
    ll end_pos = from + ((to - from + step - 1) / step) * step;
    auto begin = RangeIterator(from, step);
    auto end = RangeIterator(end_pos, step);
    return Sequence{begin, end};
}

auto Range(ll to) {
    return Range(0, to, 1);
}

// Zip
template <typename AI, typename BI>
class ZipIterator {
public:
    ZipIterator(AI ab, AI ae, BI bb, BI be, bool done)
        : a_(ab), ab_(ab), ae_(ae), b_(bb), bb_(bb), be_(be), done_(done) {
            if (ab  == ae || be == bb) {
                done_ = true;
            }
    }
    ZipIterator& operator++() {
        ++a_;
        ++b_;
        if (a_ == ae_ || b_ == be_) {
            done_ = true;
        }
        return *this;
    }
    auto operator*() const {
        return std::make_pair(*a_, *b_);
    }
    bool operator==(const ZipIterator& rhs) const {
        if (ab_ != rhs.ab_ || bb_ != rhs.bb_ || ae_ != rhs.ae_ ||
            be_ != rhs.be_) {  // iterators for different sequencies
            return false;
        }
        if (done_ != rhs.done_) {
            return false;
        }

        return done_ || (a_ == rhs.a_ && b_ == rhs.b_);
    };

private:
    AI a_;
    AI ab_;
    AI ae_;
    BI b_;
    BI bb_;
    BI be_;
    bool done_;
};

auto Zip(const auto& a, const auto& b) {
    auto begin = ZipIterator(a.begin(), a.end(), b.begin(), b.end(), false);
    auto end = ZipIterator(a.begin(), a.end(), b.begin(), b.end(), true);
    return Sequence{begin, end};
}

// Group


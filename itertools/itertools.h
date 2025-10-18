#pragma once

#include <iterator>
#include <cstddef>
#include <utility>
typedef long long ll;

template <class Iterator>
class Sequence {
public:
    Sequence(Iterator begin, Iterator end) : begin_{begin}, end_{end} {
    }
    Sequence(const Sequence& o) : begin_{o.begin_}, end_{o.end_} {
    }

    Iterator begin() const {
        return begin_;
    }

    Iterator end() const {
        return end_;
    }
    Sequence& operator=(const Sequence& o){
        begin_ = o.begin_;
        end_ = o.end_;
        return *this;
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
        if (ab == ae || be == bb) {
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
template <typename T>
class GroupElementIterator {
public:
    GroupElementIterator(T el, ll cnt) : el_(el), cnt_(cnt) {
    }

    GroupElementIterator& operator++() {
        cnt_ -= 1;
        return *this;
    }

    auto operator*() const {
        return el_;
    }

    bool operator==(const GroupElementIterator& rhs) const = default;

private:
    T el_;
    ll cnt_;
};

auto GroupElement(auto el, ll n) {
    auto begin = GroupElementIterator(el, n);
    auto end = GroupElementIterator(el, 0);
    return Sequence{begin, end};
}

template <typename AI>
class GroupIterator {
    using AEl = decltype(*std::declval<AI>());
public:
    GroupIterator(AI begin, AI end, bool is_end)
        : nxt_(begin), ab_(begin), ae_(end) {
        if (is_end || begin == end) {
            done_ = true;
            return;
        }
    }

    GroupIterator& operator++() {
        auto el = *nxt_;
        ++nxt_;
        while (nxt_ != ae_ && el == *nxt_) {
            ++nxt_;
        }
        if (nxt_ == ae_) {
            done_ = true;
        }
        return *this;
    }

    auto operator*() const {
        AI cur = nxt_;
        auto el = *cur;
        ++cur;
        int count = 1;
        while (cur != ae_ && el == *cur) {
            ++cur;
            ++count;
        }
        return GroupElement(el, count);
    }

    bool operator==(const GroupIterator& rhs) const {
        if (ab_ != rhs.ab_ || ae_ != rhs.ae_) {
            return false;
        }
        if (done_ != rhs.done_) {
            return false;
        }
        return done_ || (nxt_ == rhs.nxt_);
    }

private:
    AI nxt_;
    AI ab_;
    AI ae_;
    bool done_;
};

auto Group(const auto& a){
    auto begin = GroupIterator(a.begin(), a.end(), false);
    auto end = GroupIterator(a.begin(), a.end(), true);
    return Sequence{begin, end};
}

#pragma once

#include <map>
#include <vector>

template <typename K, typename V>
class RangeMap {
    // using K = int;
    // using V = int;
    using KeyRange = std::pair<K, K>;

public:
    RangeMap();
    RangeMap(std::initializer_list<std::pair<KeyRange, V>> l);
    void Insert(const KeyRange& key_range, const V& value);
    void Remove(const KeyRange& key_range);
    void Clear();
    size_t Size() const;
    bool Empty() const;
    bool Contains(const K& key) const;
    V Get(const K& key) const;

private:
    std::map<KeyRange, V> ranges_;
    auto LB(K k) const;
    auto UB(K k) const;
    void Split(K k);
    void Merge(K k);
};

template <typename K, typename V>
RangeMap<K, V>::RangeMap() {
}

template <typename K, typename V>
RangeMap<K, V>::RangeMap(std::initializer_list<std::pair<std::pair<K, K>, V>> l) {
    for (auto el : l) {
        Insert(el.first, el.second);
    }
}

template <typename K, typename V>
void RangeMap<K, V>::Insert(const std::pair<K, K>& key_range, const V& value) {
    auto [l, r] = key_range;
    if (l >= r) {
        throw "invalid range";
    }
    Remove(key_range);
    ranges_.insert({key_range, value});
    // Merge(l);
    // Merge(r);
}

template <typename K, typename V>
void RangeMap<K, V>::Remove(const std::pair<K, K>& key_range) {
    auto [l, r] = key_range;
    if (l >= r) {
        throw "invalid range";
    }
    Split(l);
    Split(r);
    auto b = LB(l);
    auto e = LB(r);
    std::vector<KeyRange> del;
    while (b != e) {
        del.push_back((*b).first);
        ++b;
    }
    for (auto el : del) {
        ranges_.erase(el);
    }
}

template <typename K, typename V>
void RangeMap<K, V>::Clear() {
    return ranges_.clear();
}

template <typename K, typename V>
size_t RangeMap<K, V>::Size() const {
    return ranges_.size();
}

template <typename K, typename V>
bool RangeMap<K, V>::Empty() const {
    return ranges_.empty();
}

template <typename K, typename V>
bool RangeMap<K, V>::Contains(const K& key) const {
    auto it = UB(key);
    if (it == ranges_.begin()) {
        return false;
    }
    --it;
    auto [l, r] = (*it).first;
    if (l <= key && key < r) {
        return true;
    }
    return false;
}

template <typename K, typename V>
V RangeMap<K, V>::Get(const K& key) const {
    if (!Contains(key)) {
        throw "no key";
    }
    auto it = UB(key);
    return (*--it).second;
}

template <typename K, typename V>
auto RangeMap<K, V>::LB(K k) const {
    return ranges_.upper_bound(KeyRange{k, k});
}

template <typename K, typename V>
auto RangeMap<K, V>::UB(K k) const {
    auto it = LB(k);
    if (it == ranges_.end()) {
        return it;
    }

    if ((*it).first.first == k) {
        ++it;
    }
    return it;
}

template <typename K, typename V>
void RangeMap<K, V>::Split(K k) {
    auto it = UB(k);
    if (it == ranges_.begin()) {
        return;
    }
    --it;
    auto rng = (*it).first;
    auto val = (*it).second;
    if (k <= rng.first || rng.second <= k) {
        return;
    }

    auto [l, r] = rng;
    ranges_.erase(rng);
    ranges_.insert({KeyRange{l, k}, val});
    ranges_.insert({KeyRange{k, r}, val});
}

template <typename K, typename V>
void RangeMap<K, V>::Merge(K k) {
    auto it = LB(k);
    if (it == ranges_.end()) {
        return;
    }
    if (it == ranges_.begin()) {
        return;
    }
    auto r = (*it).first;
    auto rv = (*it).second;
    --it;
    auto l = (*it).first;
    auto lv = (*it).second;
    if (rv != lv || l.second != r.first) {
        return;
    }
    ranges_.erase(r);
    ranges_.erase(l);
    ranges_.insert({{l.first, r.second}, rv});
}

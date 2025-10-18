#pragma once
#include <cstdlib>
#include <map>
#include <set>
#include <string>
template <typename Key, typename Value>
class LFUCache {
public:
    explicit LFUCache(size_t capacity) : cap_(capacity) {
    }
    LFUCache(const LFUCache& l) = delete;
    LFUCache& operator=(const LFUCache& l) = delete;

    void Set(const Key& k, const Value& v) {
        if (data_.count(k) == 0) {
            if (data_.size() == cap_) {
                Pop();
            }
        }
        data_[k] = v;
        Reg(k);
    }

    Value Get(const Key& k) {
        if (data_.count(k) == 0) {
            throw std::string("no key");
        }
        Reg(k);
        return data_[k];
    }

    bool Erase(const Key& k) {
        if (data_.count(k) == 0) {
            return false;
        }
        EraseInside(k);
        return true;
    }

private:
    std::map<Key, Value> data_;
    std::map<Key, size_t> count_;
    size_t timer_;
    std::map<Key, size_t> time_;
    size_t cap_;
    std::set<std::tuple<size_t, size_t, Key>> sorted_;

    void Reg(const Key& k) {
        auto current = std::make_tuple(count_[k], time_[k], k);
        sorted_.erase(current);
        count_[k]++;
        time_[k] = ++timer_;
        current = std::make_tuple(count_[k], time_[k], k);
        sorted_.insert(current);
    }

    void EraseInside(const Key& k) {
        auto current = std::make_tuple(count_[k], time_[k], k);
        sorted_.erase(current);
        data_.erase(k);
        time_.erase(k);
        count_.erase(k);
    }

    void Pop() {
        Key key = std::get<2>(*sorted_.begin());
        Erase(key);
    }
};

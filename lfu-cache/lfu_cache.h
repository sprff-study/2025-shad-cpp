#pragma once

template <typename Key, typename Value>
class LFUCache {
public:
    explicit LFUCache(size_t capacity);
    LFUCache(const LFUCache&);
    LFUCache& operator=(const LFUCache&);

    void Set(const Key&, const Value&);
    Value Get(const Key&);
    bool Erase(const Key&);
};

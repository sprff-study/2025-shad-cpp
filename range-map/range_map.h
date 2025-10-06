#pragma once

#include <map>

template <typename KeyType, typename ValueType>
class RangeMap {
    using KeyRange = std::pair<KeyType, KeyType>;
public:
    RangeMap();
    RangeMap(std::initializer_list<std::pair<KeyRange, ValueType>> l);

    void Insert(const KeyRange& key_range, const ValueType& value);

    void Remove(const KeyRange& key_range);

    void Clear();

    size_t Size() const;

    bool Empty() const ;

    bool Contains(const KeyType& key) const;

    ValueType Get(const KeyType& key) const;
};

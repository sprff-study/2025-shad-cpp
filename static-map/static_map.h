#pragma once

#include <vector>
#include <string>
#include <utility>
#include <algorithm>

class StaticMap {
public:
    explicit StaticMap(const std::vector<std::pair<std::string, std::string>>& items)
        : data_(items) {
        std::sort(data_.begin(), data_.end());
    }

    bool Find(const std::string& key, std::string* value) const {
        auto i = std::lower_bound(data_.begin(), data_.end(), make_pair(key, ""));
        if (i == data_.end()) {
            return false;
        }
        if (i->first != key) {
            return false;
        }
        *value = i->second;
        return true;
    }

private:
    std::vector<std::pair<std::string, std::string>> data_;
};

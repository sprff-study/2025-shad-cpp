#pragma once

#include <string>
#include <stdexcept>
#include <vector>

std::vector<std::string> Split(const std::string& string, const std::string& delimiter = " ") {
    if (string.empty()) {
        return {};
    }
    size_t pos = 0;
    std::vector<std::string> res;
    while (true) {
        size_t next = string.find(delimiter, pos);
        res.push_back(string.substr(pos, next - pos));
        if (next == std::string::npos) {
            break;
        }
        pos = next + delimiter.size();
    }
    return res;
}

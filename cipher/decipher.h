#pragma once

#include <vector>
#include <string>

std::string dec(const std::string& s, size_t c, size_t d) {

    std::string str;
    for (size_t i = 0; i < d; ++i) {
        for (size_t j = 0; j < c; ++j) {
            str += s[i + j * d];
        }
    }
    return str;
}

std::vector<std::string> Decipher(const std::string& s) {
    std::vector<std::string> res;
    size_t n = s.size();
    for (size_t d = 2; d * d <= n; ++d) {
        if (n % d != 0) {
            continue;
        }

        res.push_back(dec(s, d, n / d));
        res.push_back(dec(s, n / d, d));
    }
    return res;
}

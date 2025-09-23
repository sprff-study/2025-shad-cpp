#pragma once

#include <string>
#include <stdexcept>

std::string LongSum(const std::string& a, const std::string& b) {
    std::string res;
    int n = std::max(a.size(), b.size());
    int over = 0;
    for (int i = 0; i <= n; ++i) {
        int ac = 0, bc = 0;
        if (i < static_cast<int>(a.size())) {
            ac = a[a.size() - 1 - i] - '0';
        }
        if (i < static_cast<int>(b.size())) {
            bc = b[b.size() - 1 - i] - '0';
        }
        int x = ac + bc + over;
        res += '0' + x % 10;
        over = x / 10;
    }

    while (res.size() > 1 && res.back() == '0') {
        res.pop_back();
    }

    for (int l = 0, r = static_cast<int>(res.size()) - 1; l < r; l++, r--) {
        std::swap(res[l], res[r]);
    }

    return res;
}

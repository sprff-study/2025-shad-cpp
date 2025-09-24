#pragma once

#include <string>
#include <stdexcept>

std::string Rle(const std::string& input) {
    char last = 0;
    int cnt = 0;
    int n = input.size();
    std::string res;
    for (int i = 0; i <= n; ++i) {
        if (i == n || input[i] != last) {
            if (cnt == 1) {
                res += last;
            } else if (cnt > 1) {
                res += last;
                res += std::to_string(cnt);
            }
            if (i != n) {
                last = input[i];
                cnt = 0;
            }
        }
        cnt += 1;
    }
    return res;
}

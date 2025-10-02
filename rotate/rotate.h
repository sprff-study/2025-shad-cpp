#pragma once

#include <stdexcept>
#include <vector>
#include <cstddef>
#include <numeric>

void Rotate(std::vector<int>* data, size_t shift) {
    auto& d = *data;
    size_t n = d.size();
    size_t g = std::gcd(n, shift);
    for (size_t i = 0; i < g; ++i) {
        int start = d[i];
        size_t cur = i, nxt = (i + shift) % n;
        while (nxt != i) {
            d[cur] = d[nxt];
            cur = nxt;
            nxt = (nxt + shift) % n;
        }
        d[cur] = start;
    }
}

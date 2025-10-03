#pragma once

#include <stdexcept>
#include <cmath>

enum class RootCount { kZero, kOne, kTwo, kInf };

struct Roots {
    RootCount count;
    double first;
    double second;
};

Roots SolveQuadratic(int a, int b, int c) {
    if (a == 0) {
        if (b != 0) {
            return {RootCount::kOne, -1.0 * c / b, 0};
        }
        if (c != 0) {
            return {RootCount::kZero, 0, 0};
        }
        return {RootCount::kInf, 0, 0};
    }

    int d = 1LL * b * b - 4LL * a * c;
    if (d < 0) {
        return {RootCount::kZero, 0, 0};
    }
    double s_d = std::sqrt(1.0 * d);
    double x1 = (-b - s_d) / (2 * a);
    double x2 = (-b + s_d) / (2 * a);
    if (x1 > x2) {
        std::swap(x1, x2);
    }
    if (d == 0) {
        return {RootCount::kOne, x1, 0};
    }
    return {RootCount::kTwo, x1, x2};

    throw std::runtime_error{"Not implemented"};
}

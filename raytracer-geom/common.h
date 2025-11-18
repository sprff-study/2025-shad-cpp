#pragma once

const double kEps = 1e-7;

int Compare(double a, double b = 0) {
    a -= b;
    if (a < -kEps) {
        return -1;
    } else if (a > kEps) {
        return 1;
    }
    return 0;
}

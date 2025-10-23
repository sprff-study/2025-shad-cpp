#pragma once

const double kEps = 1e-7;

int Compare(double a) {
    if (a < -kEps) {
        return -1;
    } else if (a > kEps) {
        return 1;
    }
    return 0;
}

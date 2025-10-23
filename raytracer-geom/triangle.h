#pragma once

#include "vector.h"

#include <cstddef>

class Triangle {
public:
    Triangle(const Vector& a, const Vector& b, const Vector& c) : data_{a, b, c} {
    }

    const Vector& operator[](size_t ind) const {
        return data_[ind];
    }

    double Area() const {
        Vector e1 = data_[1] - data_[0];
        Vector e2 = data_[2] - data_[0];
        return 0.5 * Length(CrossProduct(e1, e2));
    }

private:
    std::array<Vector, 3> data_;
};

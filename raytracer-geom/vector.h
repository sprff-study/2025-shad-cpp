#pragma once

#include <array>
#include <cstddef>
#include <cmath>

class Vector {
public:
    Vector() {
        data_[0] = 0;
        data_[1] = 0;
        data_[2] = 0;
    }
    Vector(double x, double y, double z) {
        data_[0] = x;
        data_[1] = y;
        data_[2] = z;
    }

    double& operator[](size_t ind) {
        return data_[ind];
    }
    double operator[](size_t ind) const {
        return data_[ind];
    }

    void Normalize() {
        double sz = sqrt(data_[0] * data_[0] + data_[1] * data_[1] + data_[2] * data_[2]);
        data_[0] /= sz;
        data_[1] /= sz;
        data_[2] /= sz;
    }

private:
    std::array<double, 3> data_;
};

double DotProduct(const Vector& a, const Vector& b);
Vector CrossProduct(const Vector& a, const Vector& b);
double Length(const Vector& v);

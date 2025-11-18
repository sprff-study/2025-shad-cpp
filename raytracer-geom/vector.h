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

    Vector(const Vector& a, const Vector& b) {
        data_[0] = b[0] - a[0];
        data_[1] = b[1] - a[1];
        data_[2] = b[2] - a[2];
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

    Vector& operator+=(const Vector& b) {
        for (int i = 0; i < 3; ++i) {
            data_[i] += b[i];
        }
        return *this;
    }

    Vector& operator-=(const Vector& b) {
        for (int i = 0; i < 3; ++i) {
            data_[i] -= b[i];
        }
        return *this;
    }

    Vector& operator*=(const double x) {
        for (int i = 0; i < 3; ++i) {
            data_[i] *= x;
        }
        return *this;
    }

    friend Vector operator+(Vector a, const Vector& b) {
        a += b;
        return a;
    }

    friend Vector operator-(Vector a, const Vector& b) {
        a -= b;
        return a;
    }

    friend Vector operator*(Vector a, const double x) {
        a *= x;
        return a;
    }
    friend Vector operator*(Vector a, Vector b) {
        for (int i = 0; i < 3; ++i) {
            a[i] *= b[i];
        }
        return a;
    }

private:
    std::array<double, 3> data_;
};

double DotProduct(const Vector& a, const Vector& b) {
    double res = 0.0;
    for (int i = 0; i < 3; ++i) {
        res += a[i] * b[i];
    }
    return res;
}

Vector CrossProduct(const Vector& a, const Vector& b) {
    return Vector(a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0]);
}

double Length(const Vector& v) {
    return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

double Distance(const Vector& a, const Vector& b) {
    return Length(b - a);
}

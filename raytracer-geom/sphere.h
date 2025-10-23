#pragma once

#include "vector.h"

class Sphere {
public:
    Sphere(const Vector& center, double radius) : c_(center), r_(radius) {
    }

    const Vector& GetCenter() const {
        return c_;
    }
    double GetRadius() const {
        return r_;
    }

private:
    Vector c_;
    double r_;
};

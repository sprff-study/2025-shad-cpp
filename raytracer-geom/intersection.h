#pragma once

#include "vector.h"

class Intersection {
public:
    Intersection(const Vector& position, const Vector& normal, double distance)
        : pos_(position), n_(normal), d_(distance) {
        n_.Normalize();
    }

    const Vector& GetPosition() const {
        return pos_;
    }
    const Vector& GetNormal() const {
        return n_;
    }
    double GetDistance() const {
        return d_;
    }

private:
    Vector pos_;
    Vector n_;
    double d_;
};

#pragma once

#include "vector.h"

class Ray {
public:
    Ray(const Vector& origin, const Vector& direction) : o_(origin), d_(direction) {
        d_.Normalize();
    }

    const Vector& GetOrigin() const {
        return o_;
    }
    const Vector& GetDirection() const {
        return d_;
    }

private:
    Vector o_;
    Vector d_;
};

#pragma once

#include "triangle.h"
#include "material.h"
#include "sphere.h"
#include "vector.h"

struct Object {
    const Material* material = nullptr;
    Triangle polygon;
    const std::array<Vector, 3> normal;
    Object(const Triangle& s, const std::array<Vector, 3>& n, const Material* m = nullptr)
        : material(m), polygon(s), normal(n) {
    }

    const Vector* GetNormal(size_t index) const {
        return &normal[index];
    }
};

struct SphereObject {
    const Material* material = nullptr;
    Sphere sphere;
    SphereObject(const Sphere& s, const Material* m = nullptr) : material(m), sphere(s) {
    }
};

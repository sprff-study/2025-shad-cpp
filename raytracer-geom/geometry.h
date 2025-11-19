#pragma once

#include "vector.h"
#include "sphere.h"
#include "intersection.h"
#include "triangle.h"
#include "ray.h"
#include "common.h"
#include <iostream>

#include <optional>
#include <ostream>

std::optional<Intersection> GetIntersection(const Ray& ray, const Sphere& sphere) {
    const Vector& o = ray.GetOrigin();
    const Vector& d = ray.GetDirection();
    const Vector& c = sphere.GetCenter();
    double r = sphere.GetRadius();

    Vector l = o - c;

    double b = 2.0 * DotProduct(d, l);
    double cterm = DotProduct(l, l) - r * r;

    double disc = b * b - 4.0 * cterm;
    if (Compare(disc) < 0) {
        return std::nullopt;
    }

    double sqrt_disc = std::sqrt(std::max(0.0, disc));
    double t0 = (-b - sqrt_disc) * 0.5;
    double t1 = (-b + sqrt_disc) * 0.5;

    double t = t0;
    if (Compare(t) < 0) {
        t = t1;
    }
    if (Compare(t) < 0) {
        return std::nullopt;
    }

    Vector pos = o + (d * t);
    Vector n = pos - c;
    n.Normalize();

    if (DotProduct(n, d) > 0.0) {
        n *= -1.0;
    }

    return Intersection(pos, n, t);
}

std::optional<Intersection> GetIntersection(const Ray& ray, const Triangle& triangle) {
    const Vector& o = ray.GetOrigin();
    const Vector& d = ray.GetDirection();

    const Vector& a = triangle[0];
    const Vector& b = triangle[1];
    const Vector& c = triangle[2];

    Vector e1 = b - a;
    Vector e2 = c - a;

    Vector pvec = CrossProduct(d, e2);
    double det = DotProduct(e1, pvec);

    if (Compare(det) == 0) {
        return std::nullopt;
    }
    double inv_det = 1.0 / det;

    Vector tvec = o - a;
    double u = DotProduct(tvec, pvec) * inv_det;
    if (Compare(u) < 0 || Compare(u - 1) > 0) {
        return std::nullopt;
    }

    Vector qvec = CrossProduct(tvec, e1);
    double v = DotProduct(d, qvec) * inv_det;
    if (Compare(v) < 0 || Compare(u + v - 1.0) > 0) {
        return std::nullopt;
    }

    double t = DotProduct(e2, qvec) * inv_det;
    if (Compare(t) < 0) {
        return std::nullopt;
    }

    Vector pos = o + (d * t);
    Vector n = CrossProduct(e1, e2);
    n.Normalize();

    if (DotProduct(n, d) > 0.0) {
        n *= -1.0;
    }

    return Intersection(pos, n, t);
}

Vector Reflect(const Vector& ray, const Vector& normal) {
    Vector n = normal;
    n.Normalize();
    double proj = DotProduct(ray, n);
    return ray - n * (2.0 * proj);
}

std::optional<Vector> Refract(const Vector& ray, const Vector& normal, double eta) {
    Vector i = ray;
    i.Normalize();
    Vector n = normal;
    n.Normalize();

    double cosi = DotProduct(i, n);
    double eta_ratio = eta;
    if (cosi > 0.0) {
        n *= -1.0;
        cosi = DotProduct(i, n);
        eta_ratio = 1.0 / eta;
    }

    double cosi_abs = -cosi;
    double k = 1.0 - eta_ratio * eta_ratio * (1.0 - cosi_abs * cosi_abs);
    if (k < 0.0) {
        
        
        
        

        return std::nullopt;
    }

    Vector t = i * eta_ratio + n * (eta_ratio * cosi_abs - std::sqrt(k));
    return t;
}

Vector GetBarycentricCoords(const Triangle& triangle, const Vector& point) {
    const Vector& a = triangle[0];
    const Vector& b = triangle[1];
    const Vector& c = triangle[2];

    Vector v0 = b - a;
    Vector v1 = c - a;
    Vector v2 = point - a;

    double d00 = DotProduct(v0, v0);
    double d01 = DotProduct(v0, v1);
    double d11 = DotProduct(v1, v1);
    double d20 = DotProduct(v2, v0);
    double d21 = DotProduct(v2, v1);

    double denom = d00 * d11 - d01 * d01;
    if (Compare(denom) == 0) {
        return Vector(1.0, 0.0, 0.0);
    }

    double v = (d11 * d20 - d01 * d21) / denom;
    double w = (d00 * d21 - d01 * d20) / denom;
    double u = 1.0 - v - w;

    return Vector(u, v, w);
}

Vector GetNormal(Vector a, Vector b, Vector c) {
    Vector n = CrossProduct({a, b}, {a, c});
    if (Compare(Length(n)) == 0) {
        throw "can't get notmal for 3 points in one line";
    }
    n.Normalize();
    return n;
}

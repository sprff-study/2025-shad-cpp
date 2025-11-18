#pragma once

#include "geometry.h"
#include "material.h"
#include "options/camera_options.h"
#include "options/render_options.h"
#include "image.h"
#include "floating_image.h"
#include "ray.h"
#include "common.h"
#include "scene.h"
#include "vector.h"

#include <filesystem>
#include <filesystem>
#include <optional>
// #include <chrono>

// std::chrono::steady_clock::time_point start, last;
// void bench(const std::string& label = "") {
//     auto now = std::chrono::steady_clock::now();
//     std::cout << label << ": ";
//     std::cout << "start ("
//               << std::chrono::duration_cast<std::chrono::nanoseconds>(now - start).count()
//               << "[ns]) ";
//     std::cout << "last ("
//               << std::chrono::duration_cast<std::chrono::nanoseconds>(now - last).count()
//               << "[ns])\n";
//     last = now;
// }

struct PreparedCameraOptions {
    CameraOptions options;
    PreparedCameraOptions(const CameraOptions& c) : options(c) {
        hszy_ = tan(c.fov / 2);
        hszx_ = hszy_ / c.screen_height * c.screen_width;
        f_ = c.look_to - c.look_from;
        f_.Normalize();

        Vector up = Vector(0, 1, 0);
        if (Compare(DotProduct(f_, up) - 1) == 0) {
            up = Vector(0, 0, 1);
        } else if (Compare(DotProduct(f_, up) + 1) == 0) {
            up = Vector(0, 0, -1);
        }
        r_ = CrossProduct(f_, up);
        r_.Normalize();
        u_ = CrossProduct(r_, f_);
        u_.Normalize();
    }

    Ray EmitRay(int i, int j) const {
        double y = (1.0 * i + 0.5) / options.screen_height;
        double x = (1.0 * j + 0.5) / options.screen_width;
        double cy = hszy_ - 2.0 * hszy_ * y;
        double cx = -hszx_ + 2.0 * hszx_ * x;
        return Ray(options.look_from, r_ * cx + u_ * cy + f_);
    }

private:
    Vector f_, r_, u_;
    double hszy_, hszx_;
};

struct ShotResult {
    double distance{-1};
    Vector point{-1, -2, -3};
    Vector n{-1, -2, -3};
    const Material* material;
    Ray original;
};

std::optional<ShotResult> Shot(const Scene& scene, Ray ray) {
    std::optional<ShotResult> res = std::nullopt;

    for (auto t : scene.GetObjects()) {
        auto inter = GetIntersection(ray, t.polygon);
        if (!inter) {
            continue;
        }
        double x = inter->GetDistance();
        if (res && res->distance < x) {
            continue;
        }

        Vector def = inter->GetNormal();
        Vector bc = GetBarycentricCoords(t.polygon, inter->GetPosition());
        Vector n0 = *t.GetNormal(0);
        Vector n1 = *t.GetNormal(1);
        Vector n2 = *t.GetNormal(2);

        Vector ns = n0 * bc[0] + n1 * bc[1] + n2 * bc[2];
        if (Compare(Length(ns)) == 0) {
            ns = def;
        } else {
            ns.Normalize();
        }

        if (DotProduct(ns, def) < 0.0) {
            ns *= -1.0;
        }
        res = ShotResult{.distance = x,
                         .point = inter->GetPosition(),
                         .n = ns,
                         .material = t.material,
                         .original = ray};
    }

    for (auto s : scene.GetSphereObjects()) {
        auto inter = GetIntersection(ray, s.sphere);
        if (!inter) {
            continue;
        }
        double x = (*inter).GetDistance();
        if (res && res->distance < x) {
            continue;
        }
        res = ShotResult{.distance = x,
                         .point = inter->GetPosition(),
                         .n = inter->GetNormal(),
                         .material = s.material,
                         .original = ray};
    }
    return res;
}

Vector ColorInPoint(const Scene& scene, ShotResult shr) {
    auto p = shr.point;
    auto n = shr.n;
    auto m = shr.material;

    Vector diffuse{}, specular{};

    for (auto light : scene.GetLights()) {
        Vector ray_origin = p + n * kEps;
        Vector ray_direction = light.position - ray_origin;
        double light_distance = Distance(ray_origin, light.position);
        auto shr2 = Shot(scene, Ray{ray_origin, ray_direction});
        if (shr2 && Compare(shr2->distance, light_distance) < 0) {
            continue;
        }

        {
            Vector v = {p, light.position};
            v.Normalize();
            double dot = std::max(0.0, DotProduct(v, n));
            diffuse += light.intensity * dot;  
        }

        {
            Vector vr = shr.original.GetDirection();
            vr.Normalize();
            Vector vlr = Reflect(Vector{light.position, p}, n);
            vlr.Normalize();
            double dot = std::max(0.0, DotProduct(vr, vlr));
            dot = pow(dot, m->specular_exponent);
            specular += light.intensity * dot;
        }
    }
    Vector res = m->diffuse_color * diffuse;
    res += m->specular_color * specular;
    res *= m->albedo[0];
    res += m->ambient_color;
    res += m->intensity;

    return res;
}

Image RenderFull(const Scene& scene, const PreparedCameraOptions& camera_options) {
    FloatingImage res(camera_options.options.screen_width, camera_options.options.screen_height);
    for (int i = 0; i < camera_options.options.screen_height; ++i) {
        for (int j = 0; j < camera_options.options.screen_width; ++j) {
            auto shr = Shot(scene, camera_options.EmitRay(i, j));
            Vector color = Vector(0, 0, 0);
            if (shr) {
                color = ColorInPoint(scene, *shr);
            }
            res.SetPixel(i, j, FloatingRGB{color[0], color[1], color[2]});
        }
    }
    res.ToneMapping();
    res.GammaCorrection();

    return res.ToImage();
}
Image RenderDepth(const Scene& scene, const PreparedCameraOptions& camera_options) {
    FloatingImage res(camera_options.options.screen_width, camera_options.options.screen_height);
    double dmax = 0;
    for (int i = 0; i < camera_options.options.screen_height; ++i) {
        // bench("finish row");
        for (int j = 0; j < camera_options.options.screen_width; ++j) {
            auto ray = camera_options.EmitRay(i, j);
            // bench("ray emmited");
            auto shr = Shot(scene, ray);
            if (shr) {
                dmax = std::max(dmax, shr->distance);
            }
            // bench("distance");
        }
    }
    assert(Compare(dmax) > 0);
    for (int i = 0; i < camera_options.options.screen_height; ++i) {
        for (int j = 0; j < camera_options.options.screen_width; ++j) {
            auto shr = Shot(scene, camera_options.EmitRay(i, j));
            double d = dmax;
            if (shr) {
                d = shr->distance;
            }
            d /= dmax;
            res.SetPixel(i, j, FloatingRGB{d, d, d});
        }
    }

    return res.ToImage();
}

Image RenderNormal(const Scene& scene, const PreparedCameraOptions& camera_options) {
    FloatingImage res(camera_options.options.screen_width, camera_options.options.screen_height);
    for (int i = 0; i < camera_options.options.screen_height; ++i) {
        for (int j = 0; j < camera_options.options.screen_width; ++j) {
            auto shr = Shot(scene, camera_options.EmitRay(i, j));
            if (shr) {
                Vector n = shr->n;
                res.SetPixel(i, j,
                             FloatingRGB{n[0] / 2.0 + 0.5, n[1] / 2.0 + 0.5, n[2] / 2.0 + 0.5});
            } else {
                res.SetPixel(i, j, FloatingRGB{0, 0, 0});
            }
        }
    }
    return res.ToImage();
}

Image Render(const std::filesystem::path& path, const CameraOptions& camera_options,
             const RenderOptions& render_options) {
    // start = std::chrono::steady_clock::now();
    // last = std::chrono::steady_clock::now();
    PreparedCameraOptions prep{camera_options};
    auto scene = ReadScene(path);
    // bench("Read Scene");
    if (render_options.mode == RenderMode::kDepth) {
        return RenderDepth(scene, prep);
    }
    if (render_options.mode == RenderMode::kNormal) {
        return RenderNormal(scene, prep);
    }
    if (render_options.mode == RenderMode::kFull) {
        return RenderFull(scene, prep);
    }

    return Image{camera_options.screen_width, camera_options.screen_height};
}

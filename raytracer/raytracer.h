#pragma once

#include "options/camera_options.h"
#include "options/render_options.h"
#include "image.h"
#include "floating_image.h"
#include "scene.h"

#include <filesystem>
#include <filesystem>

struct PreparedCameraOptions {
    CameraOptions options;
    PreparedCameraOptions(const CameraOptions& c) : options(c) {
        hszy = tan(c.fov / 2);
        hszx = hszy / c.screen_height * c.screen_width;
        f = c.look_to - c.look_from;
        f.Normalize();

        Vector up = Vector(0, 1, 0);
        if (Compare(DotProduct(f, up) - 1) == 0) {
            up = Vector(0, 0, 1);
        } else if (Compare(DotProduct(f, up) + 1) == 0) {
            up = Vector(0, 0, -1);
        }
        r = CrossProduct(f, up);
        r.Normalize();
        u = CrossProduct(r, f);
        u.Normalize();
    }

    Ray EmitRay(int i, int j) const {
        double y = (1.0 * i + 0.5) / options.screen_height;
        double x = (1.0 * j + 0.5) / options.screen_width;
        double cy = hszy - 2.0 * hszy * y;
        double cx = -hszx + 2.0 * hszx * x;
        return Ray(options.look_from, r * cx + u * cy + f);
    }

private:
    Vector f, r, u;
    double hszy, hszx;
};

double Distance(const Scene& scene, Ray ray) {
    double d = -1;
    for (auto t : scene.GetObjects()) {
        auto inter = GetIntersection(ray, t.polygon);
        if (!inter) {
            continue;
        }
        double x = (*inter).GetDistance();
        if (d == -1 || d > x) {
            d = x;
        }
    }
    for (auto s : scene.GetSphereObjects()) {
        auto inter = GetIntersection(ray, s.sphere);
        if (!inter) {
            continue;
        }
        double x = (*inter).GetDistance();
        if (d == -1 || d > x) {
            d = x;
        }
    }
    return d;
}

Image RenderDepth(const Scene& scene, const PreparedCameraOptions& camera_options) {
    FloatingImage res(camera_options.options.screen_width, camera_options.options.screen_height);
    double dmax = 0;
    for (int i = 0; i < camera_options.options.screen_height; ++i) {
        for (int j = 0; j < camera_options.options.screen_width; ++j) {
            Ray _ = camera_options.EmitRay(i, j);
            double d = 123;
            dmax = std::max(dmax, d);
        }
    }
    assert(Compare(dmax) > 0);
    for (int i = 0; i < camera_options.options.screen_height; ++i) {
        for (int j = 0; j < camera_options.options.screen_width; ++j) {
            double d = Distance(scene, camera_options.EmitRay(i, j));
            if (d == -1) {
                d = dmax;
            }
            d /= dmax;
            res.SetPixel(i, j, FloatingRGB{d, d, d});
        }
    }

    return res.ToImage();
}

Vector Normal(const Scene& scene, Ray ray) {
    double d = -1;
    Vector n{-1, -1, -1};

    for (auto t : scene.GetObjects()) {
        auto inter = GetIntersection(ray, t.polygon);
        if (!inter) {
            continue;
        }
        double x = (*inter).GetDistance();
        if (d == -1 || d > x) {
            d = x;
            n = (*inter).GetNormal();
        }
    }
    for (auto s : scene.GetSphereObjects()) {
        auto inter = GetIntersection(ray, s.sphere);
        if (!inter) {
            continue;
        }
        double x = (*inter).GetDistance();
        if (d == -1 || d > x) {
            d = x;
            n = (*inter).GetNormal();
        }
    }
    return n;
}

Image RenderNormal(const Scene& scene, const PreparedCameraOptions& camera_options) {
    FloatingImage res(camera_options.options.screen_width, camera_options.options.screen_height);
    for (int i = 0; i < camera_options.options.screen_height; ++i) {
        for (int j = 0; j < camera_options.options.screen_width; ++j) {
            Vector n = Normal(scene, camera_options.EmitRay(i, j));
            res.SetPixel(i, j, FloatingRGB{n[0] / 2.0 + 0.5, n[1] / 2.0 + 0.5, n[2] / 2.0 + 0.5});
        }
    }
    return res.ToImage();
}

Image Render(const std::filesystem::path& path, const CameraOptions& camera_options,
             const RenderOptions& render_options) {
    PreparedCameraOptions prep{camera_options};
    auto scene = ReadScene(path);
    if (render_options.mode == RenderMode::kDepth) {
        return RenderDepth(scene, prep);
    }
    if (render_options.mode == RenderMode::kNormal) {
        return RenderNormal(scene, prep);
    }

    return Image{camera_options.screen_width, camera_options.screen_height};
}

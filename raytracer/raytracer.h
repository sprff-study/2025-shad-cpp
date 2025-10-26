#pragma once

#include "options/camera_options.h"
#include "options/render_options.h"
#include "image.h"
#include "floating_image.h"
#include "scene.h"

#include <filesystem>

Vector CamToWorld(const Vector& pos_cam, const Vector& from, const Vector& to) {
    Vector F = to - from;
    F.Normalize();

    Vector up = Vector(0, 1, 0);
    if (Compare(DotProduct(F, up) - 1) == 0) {
        up = Vector(0, 0, 1);
    } else if (Compare(DotProduct(F, up) + 1) == 0) {
        up = Vector(0, 0, -1);
    }

    Vector R = CrossProduct(F, up);
    R.Normalize();

    Vector U = CrossProduct(R, F);
    U.Normalize();

    const double x = pos_cam[0];
    const double y = pos_cam[1];
    const double z = pos_cam[2];

    return R * x + U * y - F * z;
}

Ray EmitRay(const CameraOptions& camera_options, int i, int j) {
    double length = 1;
    double y = (1.0 * i + 0.5) / camera_options.screen_height;
    double x = (1.0 * j + 0.5) / camera_options.screen_width;
    double hszy = length * tan(camera_options.fov / 2);
    double hszx = hszy / camera_options.screen_height * camera_options.screen_width;
    double cy = hszy - 2.0 * hszy * y;
    double cx = -hszx + 2.0 * hszx * x;

    Vector cameraPos = Vector(cx, cy, -1);
    Vector from = camera_options.look_from;
    Vector to = camera_options.look_to;

    return Ray(from, CamToWorld(cameraPos, from, to));
}

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

Image RenderDepth(const Scene& scene, const CameraOptions& camera_options) {
    FloatingImage res(camera_options.screen_width, camera_options.screen_height);
    double D = 0;
    for (int i = 0; i < camera_options.screen_height; ++i) {
        for (int j = 0; j < camera_options.screen_width; ++j) {
            double d = Distance(scene, EmitRay(camera_options, i, j));
            D = std::max(D, d);
        }
    }
    assert(Compare(D) > 0);
    for (int i = 0; i < camera_options.screen_height; ++i) {
        for (int j = 0; j < camera_options.screen_width; ++j) {
            double d = Distance(scene, EmitRay(camera_options, i, j));
            if (d == -1) {
                d = D;
            }
            d /= D;
            res.SetPixel(i, j, FloatingRGB{d, d, d});
        }
    }
    return res.ToImage();
}

Vector Normal(const Scene& scene, Ray ray) {
    double d = -1;
    Vector n{-1,-1,-1};

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

Image RenderNormal(const Scene& scene, const CameraOptions& camera_options) {
    FloatingImage res(camera_options.screen_width, camera_options.screen_height);
    for (int i = 0; i < camera_options.screen_height; ++i) {
        for (int j = 0; j < camera_options.screen_width; ++j) {
            Vector n = Normal(scene, EmitRay(camera_options, i, j));
            res.SetPixel(i, j, FloatingRGB{n[0]/2.0 + 0.5, n[1]/2.0 + 0.5, n[2]/2.0 + 0.5});
        }
    }
    return res.ToImage();
}

Image Render(const std::filesystem::path& path, const CameraOptions& camera_options,
             const RenderOptions& render_options) {
    auto scene = ReadScene(path);
    if (render_options.mode == RenderMode::kDepth) {
        return RenderDepth(scene, camera_options);
    }
    if (render_options.mode == RenderMode::kNormal) {
        return RenderNormal(scene, camera_options);
    }

    return Image{camera_options.screen_width, camera_options.screen_height};
}

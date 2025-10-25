#pragma once

#include "options/camera_options.h"
#include "options/render_options.h"
#include "image.h"
#include "floating_image.h"
#include "scene.h"

#include <filesystem>

FloatingRGB RenderDepth(Scene& scene, Vector o, Vector p) {

} 


Image Render(const std::filesystem::path& path, const CameraOptions& camera_options,
             const RenderOptions& render_options) {
    int width = camera_options.screen_width;
    int height = camera_options.screen_height;
    if (render_options.mode == RenderMode::kFull) {
        return Image(width, height);
    }
    auto scene = ReadScene(path);
    FloatingImage result(width, height);
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            double m = 1.0 * i / height;
            result.SetPixel(i, j, FloatingRGB{m, m, m});
        }
    }
    return result.Postprocess();
}

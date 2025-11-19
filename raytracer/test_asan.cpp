#include "options/camera_options.h"
#include "options/render_options.h"
#include "tests/commons.h"
#include "raytracer.h"
#include "utils.h"
#include "image.h"

#include <cmath>
#include <string_view>
#include <optional>
#include <numbers>

#include <catch2/catch_test_macros.hpp>

void CheckImage(std::string_view obj_filename, std::string_view result_filename,
                const CameraOptions& camera_options, const RenderOptions& render_options,
                const std::optional<std::filesystem::path>& output_path = std::nullopt) {
    static const auto kTestsDir = GetRelativeDir(__FILE__, "tests");
    auto image = Render(kTestsDir / obj_filename, camera_options, render_options);
    if (output_path) {
        image.Write(*output_path);
    }
    Compare(image, Image{kTestsDir / result_filename});
}

// TEST_CASE("Shading parts") {
//     std::cout << "Shading parts" << std::endl;
//     CameraOptions camera_opts{640, 480};
//     CheckImage("shading_parts/scene.obj", "shading_parts/scene.png", camera_opts, {1},
//     "../raytracer/debug/shading_parts_debug.png");
// }

// TEST_CASE("Triangle") {
//     std::cout << "Triangle" << std::endl;
//     CameraOptions camera_opts{.screen_width = 640,
//                               .screen_height = 480,
//                               .look_from = {0., 2., 0.},
//                               .look_to = {0., 0., 0.}};
//     CheckImage("triangle/scene.obj", "triangle/scene.png", camera_opts, {1},
//         "../raytracer/debug/triangle_debug.png");
// }

// TEST_CASE("Triangle2") {
//     std::cout << "Triangle2" << std::endl;
//     CameraOptions camera_opts{.screen_width = 640,
//                               .screen_height = 480,
//                               .look_from = {0., -2., 0.},
//                               .look_to = {0., 0., 0.}};
//     CheckImage("triangle/scene.obj", "triangle/black.png", camera_opts, {1},
//         "../raytracer/debug/triangle2_debug.png");    
// }

TEST_CASE("Box with spheres") {
    std::cout << "Box with spheres" << std::endl;
    CameraOptions camera_opts{.screen_width = 640,
                              .screen_height = 480,
                              .fov = std::numbers::pi / 3,
                              .look_from = {0., .7, 1.75},
                              .look_to = {0., .7, 0.}};
    CheckImage("box/cube.obj", "box/cube.png", camera_opts, {4},
        "../raytracer/debug/cube_debug.png");
}

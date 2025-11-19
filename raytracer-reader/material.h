#pragma once

#include "vector.h"

#include <string>

struct Material {
    std::string name;
    Vector ambient_color;        // Ka
    Vector diffuse_color;        // Kd
    Vector specular_color;       // Ks
    Vector intensity;            // Ke
    double specular_exponent;    // Ns
    double refraction_index{1};  // Ni
    Vector albedo{1, 0, 0};      // al
};

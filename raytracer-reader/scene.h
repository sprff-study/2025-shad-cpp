#pragma once

#include "material.h"
#include "vector.h"
#include "object.h"
#include "light.h"
#include "read.h"

#include <vector>
#include <unordered_map>
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>

class Scene {
public:
    Scene(std::vector<Object>&& objects, std::vector<SphereObject>&& spheres,
          std::vector<Light>&& lights, std::unordered_map<std::string, Material>&& materials)
        : objects_(std::move(objects)),
          spheres_(std::move(spheres)),
          lights_(std::move(lights)),
          materials_(std::move(materials)) {
    }
    const std::vector<Object>& GetObjects() const {
        return objects_;
    }
    const std::vector<SphereObject>& GetSphereObjects() const {
        return spheres_;
    }
    const std::vector<Light>& GetLights() const {
        return lights_;
    }
    const std::unordered_map<std::string, Material>& GetMaterials() const {
        return materials_;
    }

private:
    std::vector<Object> objects_;
    std::vector<SphereObject> spheres_;
    std::vector<Light> lights_;
    std::unordered_map<std::string, Material> materials_;
};

std::unordered_map<std::string, Material> ReadMaterials(const std::filesystem::path& path) {
    // auto&  logger = std::cout;
    std::unordered_map<std::string, Material> res;
    std::ifstream file(path.c_str(), std::ios::in);
    std::string line;

    Material current;
    current.name = "";

    while (std::getline(file, line)) {
        std::istringstream in(line);
        std::string type;
        if (!(in >> type)) {
            continue;
        }

        if (!type.empty() && type[0] == '#') {
            continue;
        }

        // logger << type << std::endl;
        if (type == "newmtl") {
            if (!current.name.empty()) {
                res[current.name] = current;
            }
            current = {};
            in >> current.name;
        } else if (type == "Ka") {
            current.ambient_color = ReadVector(in);
        } else if (type == "Ke") {
            current.intensity = ReadVector(in);
        } else if (type == "Kd") {
            current.diffuse_color = ReadVector(in);
        } else if (type == "Ks") {
            current.specular_color = ReadVector(in);

        } else if (type == "Ns") {
            in >> current.specular_exponent;
        } else if (type == "Ni") {
            in >> current.refraction_index;
        } else if (type == "al") {
            current.albedo = ReadVector(in);
        } else {
            {
                // logger << type << "SKIPPED" << std::endl;
            }
        }
    }

    if (!current.name.empty()) {
        res[current.name] = current;
    }
    return res;
}

Scene ReadScene(const std::filesystem::path& path) {
    // auto&  logger = std::cout;
    std::vector<Object> objects;
    std::vector<SphereObject> spheres;
    std::vector<Light> lights;
    std::unordered_map<std::string, Material> materials;

    std::vector<Vector> vertexes;
    std::vector<Vector> normals;

    std::ifstream file(path.c_str(), std::ios::in);
    std::string line;
    Material* current_material = nullptr;

    while (std::getline(file, line)) {
        std::istringstream in(line);
        std::string type;
        if (!(in >> type)) {
            continue;
        }
        if (!type.empty() && type[0] == '#') {
            continue;
        }

        // logger << type << std::endl;
        if (type == "mtllib") {
            std::string name;
            in >> name;
            std::filesystem::path newpath(path);
            newpath.replace_filename(name);
            auto mats = ReadMaterials(newpath);
            for (auto [k, v] : mats) {
                // logger << "add material: " << k << std::endl;
                materials[k] = v;
            }
        } else if (type == "usemtl") {
            std::string name;
            in >> name;
            // logger << "current material: " << name << std::endl;
            if (materials.count(name) == 0) {
                throw "unexpected material";
            }
            current_material = &materials[name];
        } else if (type == "S") {
            auto s = ReadSphere(in);
            s.material = current_material;
            spheres.push_back(s);
            // logger << "sphere added" << std::endl;
        } else if (type == "v") {
            vertexes.push_back(ReadVector(in));
            // logger << "vertex added" << std::endl;
        } else if (type == "vn") {
            normals.push_back(ReadVector(in));
            // logger << "normal added" << std::endl;
        } else if (type == "f") {
            auto triangles = ReadObject(in, vertexes, normals);
            for (auto t : triangles) {
                t.material = current_material;
                objects.push_back(t);
                // logger << "object added" << std::endl;
            }
        } else if (type == "P") {
            lights.push_back(ReadLight(in));
            // logger << "light added" << std::endl;

        } else {
            // logger << type << " SKIPPED" << std::endl;
        }
    }
    return Scene{std::move(objects), std::move(spheres), std::move(lights), std::move(materials)};
}

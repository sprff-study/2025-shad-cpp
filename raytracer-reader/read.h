#pragma once

#include "material.h"
#include "vector.h"
#include "object.h"
#include "light.h"
#include "geometry.h"

#include <vector>
#include <unordered_map>
#include <string>
#include <filesystem>
#include <sstream>

void NegativeIndex(int& x, int n) {
    if (x < 0) {
        x += 1 + n;
    }
}

std::tuple<int, int, int> ParseNode(const std::string& node) {
    int a = 0, b = 0, c = 0;
    std::istringstream in(node);
    if (!(in >> a)) {
        return {0, 0, 0};
    }
    in >> std::ws;
    if (in.peek() == '/') {
        in.get();

        in >> std::ws;
        if (in.peek() == '/') {
            in.get();
            (in >> c);
        } else {
            if (in >> b) {
                in >> std::ws;
                if (in.peek() == '/') {
                    in.get();
                    (in >> c);
                }
            }
        }
    }
    return {a, b, c};
}

std::vector<Object> ReadObject(std::istringstream& in, const std::vector<Vector>& verexes,
                               const std::vector<Vector>& normals) {
    std::vector<Object> res;

    std::vector<std::string> nodes;
    std::string node;
    while (in >> node) {
        nodes.push_back(node);
    }
    int n = nodes.size();
    if (n < 3) {
        throw "f must contain at least 3 nodes";
    }
    for (int i = 1; i + 1 < n; ++i) {
        auto [v1, c1, n1] = ParseNode(nodes[0]);
        auto [v2, c2, n2] = ParseNode(nodes[i]);
        auto [v3, c3, n3] = ParseNode(nodes[i + 1]);
        NegativeIndex(n1, normals.size());
        NegativeIndex(n2, normals.size());
        NegativeIndex(n3, normals.size());
        NegativeIndex(v1, verexes.size());
        NegativeIndex(v2, verexes.size());
        NegativeIndex(v3, verexes.size());
        auto a = verexes[v1 - 1];
        auto b = verexes[v2 - 1];
        auto c = verexes[v3 - 1];
        Triangle t(a, b, c);
        std::array<Vector, 3> nrms;
        if (n1 != 0) {
            nrms[0] = normals[n1 - 1];
            nrms[1] = normals[n2 - 1];
            nrms[2] = normals[n3 - 1];
        } else {
            auto n = GetNormal(a, b, c);
            nrms[0] = n;
            nrms[1] = n;
            nrms[2] = n;
        }
        res.push_back(Object(t, nrms));
    }
    return res;
}

SphereObject ReadSphere(std::istringstream& in) {
    double x, y, z, r;
    in >> x >> y >> z >> r;
    return SphereObject(Sphere(Vector(x, y, z), r));
}

Vector ReadVector(std::istringstream& in) {
    double x, y, z;
    in >> x >> y >> z;
    return Vector(x, y, z);
}

Light ReadLight(std::istringstream& in) {
    Vector positiion = ReadVector(in), intensity = ReadVector(in);
    return Light{positiion, intensity};
}

#pragma once
#include <cassert>
#include <cmath>
#include <algorithm>

#include "image.h"

struct FloatingRGB {
    double r, g, b;
};
class FloatingImage {

public:
    FloatingImage(int width, int height) : width_(width), height_(height), data_(width * height) {
    }
    FloatingRGB GetPixel(int i, int j) {
        assert(0 <= i && i < height_ && "out of bounds");
        assert(0 <= j && j < width_ && "out of bounds");
        return data_[i * width_ + j];
    }

    void SetPixel(int i, int j, FloatingRGB x) {
        assert(0 <= i && i < height_ && "out of bounds");
        assert(0 <= j && j < width_ && "out of bounds");
        data_[i * width_ + j] = x;
    }

    Image Postprocess() {
        Image post(width_, height_);
        double C = 0;
        for (int i = 0; i < height_; ++i) {
            for (int j = 0; j < width_; ++j) {
                FloatingRGB fx = data_[i * width_ + j];
                C = std::max({C, fx.r, fx.g, fx.b});
            }
        }
        C *= C;
        for (int i = 0; i < height_; ++i) {
            for (int j = 0; j < width_; ++j) {
                FloatingRGB fx = data_[i * width_ + j];
                auto toneMap = [&C](double x) { return x * (1 + x / C) / (1 + x); };
                auto gammaCorrection = [](double x) { return pow(x, 1.0 / 2.2); };

                int r = 255 * gammaCorrection(toneMap(fx.r));
                int g = 255 * gammaCorrection(toneMap(fx.g));
                int b = 255 * gammaCorrection(toneMap(fx.b));

                post.SetPixel({r, g, b}, i, j);
            }
        }
        return post;
    }

private:
    int width_;
    int height_;
    std::vector<FloatingRGB> data_;
};

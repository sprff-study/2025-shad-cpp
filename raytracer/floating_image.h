#pragma once
#include <cassert>
#include <cmath>
#include <algorithm>
#include <vector>

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

    void ToneMapping() {
        double c = 0;
        for (int i = 0; i < height_; ++i) {
            for (int j = 0; j < width_; ++j) {
                FloatingRGB fx = data_[i * width_ + j];
                c = std::max({c, fx.r, fx.g, fx.b});
            }
        }
        c *= c;
        for (int i = 0; i < height_; ++i) {
            for (int j = 0; j < width_; ++j) {
                FloatingRGB fx = data_[i * width_ + j];
                auto tone_map = [&c](double x) { return x * (1 + x / c) / (1 + x); };
                data_[i * width_ + j] = FloatingRGB{
                    tone_map(fx.r),
                    tone_map(fx.g),
                    tone_map(fx.b),
                };
            }
        }
    }

    void GammaCorrection() {
        for (int i = 0; i < height_; ++i) {
            for (int j = 0; j < width_; ++j) {
                FloatingRGB fx = data_[i * width_ + j];
                auto gamma = [](double x) { return pow(x, 1.0 / 2.2); };
                data_[i * width_ + j] = FloatingRGB{
                    gamma(fx.r),
                    gamma(fx.g),
                    gamma(fx.b),
                };
            }
        }
    }

    Image ToImage() {
        Image post(width_, height_);
        for (int i = 0; i < height_; ++i) {
            for (int j = 0; j < width_; ++j) {
                FloatingRGB fx = data_[i * width_ + j];
                int r = 255 * ((std::isnan(fx.r) || std::isinf(fx.r)) ? 0.0 : fx.r);
                if (r > 255) {
                    r = 255;
                }
                if (r < 0) {
                    r = 0;
                }
                int g = 255 * ((std::isnan(fx.g) || std::isinf(fx.g)) ? 0.0 : fx.g);
                if (g > 255) {
                    g = 255;
                }
                if (g < 0) {
                    g = 0;
                }
                int b = 255 * ((std::isnan(fx.b) || std::isinf(fx.b)) ? 0.0 : fx.b);
                if (b > 255) {
                    b = 255;
                }
                if (b < 0) {
                    b = 0;
                }
                assert(0 <= r && r <= 255);
                assert(0 <= g && g <= 255);
                assert(0 <= b && b <= 255);

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

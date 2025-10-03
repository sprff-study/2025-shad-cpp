#pragma once

#include <cstddef>
#include <vector>

class RingBuffer {
public:
    explicit RingBuffer(size_t capacity) : i_(0), sz_(0), n_(capacity), data_(capacity) {
    }

    size_t Size() const {
        return sz_;
    }

    bool Empty() const {
        return sz_ == 0;
    }

    bool TryPush(int element) {
        if (sz_ == n_) {
            return false;
        }
        data_[(i_ + sz_) % n_] = element;
        sz_ += 1;
        return true;
    }

    bool TryPop(int* element) {
        if (sz_ == 0) {
            return false;
        }
        *element = data_[i_];
        sz_ -= 1;
        i_ = (i_ + 1) % n_;
        return true;
    }

private:
    int i_;
    size_t sz_;
    size_t n_;
    std::vector<int> data_;
};

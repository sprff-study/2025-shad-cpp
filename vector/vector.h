#pragma once

#include <iterator>
#include <cstddef>
#include <algorithm>

class Vector {
public:
    class Iterator {
    public:
        using iterator_category = std::contiguous_iterator_tag;
        using value_type = int;
        using difference_type = ptrdiff_t;
        using pointer = int*;
        using reference = int&;

        Iterator() = default;
        Iterator(int i, int sz, int* data) : i_(i), sz_(sz), data_(data) {
        }

        pointer operator->() const {
            return data_ + i_;
        }
        reference operator*() const {
            return data_[i_];
        }
        Iterator& operator++() {
            i_++;
            if (i_ > sz_) {
                i_ = sz_;
            }
            return *this;
        }
        Iterator operator++(int) {
            Iterator res = *this;
            i_++;
            if (i_ > sz_) {
                i_ = sz_;
            }
            return res;
        }
        Iterator& operator--() {
            i_--;
            return *this;
        }
        Iterator operator--(int) {
            Iterator res = *this;
            i_--;
            return res;
        }
        Iterator& operator+=(int n) {
            i_ += n;
            if (i_ > sz_) {
                i_ = sz_;
            }
            return *this;
        }
        friend Iterator operator+(Iterator a, int n) {
            a += n;
            return a;
        }
        friend Iterator operator+(int n, Iterator a) {
            a += n;
            return a;
        }
        Iterator& operator-=(int n) {
            i_ -= n;
            return *this;
        }
        friend Iterator operator-(Iterator a, int n) {
            a -= n;
            return a;
        }
        difference_type operator-(Iterator b) const {
            return i_ - b.i_;
        }
        reference operator[](int i) const {
            return data_[i + i_];
        }
        auto operator<=>(const Iterator& other) const = default;

    private:
        int i_{0};
        int sz_{0};
        int* data_{nullptr};
    };

    Vector() {
    }
    Vector(int n) : data_(new int[n]()), size_(n), cap_(n) {
    }
    Vector(const Vector& other) {
        data_ = new int[other.size_]();
        size_ = other.size_;
        cap_ = other.cap_;
        for (size_t i = 0; i < size_; ++i) {
            data_[i] = other.data_[i];
        }
    }
    Vector(Vector&& other) {
        data_ = other.data_;
        size_ = other.size_;
        cap_ = other.cap_;

        other.data_ = nullptr;
        other.size_ = 0;
        other.cap_ = 0;
    }
    Vector& operator=(const Vector& other) {
        if (this == &other) {
            return *this;
        }
        delete[] data_;
        data_ = new int[other.size_]();
        size_ = other.size_;
        cap_ = other.cap_;
        for (size_t i = 0; i < size_; ++i) {
            data_[i] = other.data_[i];
        }
        return *this;
    }
    Vector& operator=(Vector&& other) {
        if (this == &other) {
            return *this;
        }
        delete[] data_;
        data_ = other.data_;
        size_ = other.size_;
        cap_ = other.cap_;

        other.data_ = nullptr;
        other.size_ = 0;
        other.cap_ = 0;

        return *this;
    }

    Vector(std::initializer_list<int> values) {
        data_ = new int[values.size()]();
        size_ = values.size();
        cap_ = size_;
        int i = 0;
        for (auto x : values) {
            data_[i] = x;
            ++i;
        }
    }
    ~Vector() {
        delete[] data_;
        size_ = 0;
        cap_ = 0;
    }

    void Swap(Vector& other) {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(cap_, other.cap_);
    }

    int& operator[](int i) {
        return data_[i];
    }
    const int& operator[](int i) const {
        return data_[i];
    }
    size_t Size() const {
        return size_;
    }
    size_t Capacity() const {
        return cap_;
    }

    void PushBack(int x) {
        if (size_ == cap_) {
            Reserve(std::max(2 * cap_, static_cast<size_t>(1)));
        }
        data_[size_++] = x;
    }
    void PopBack() {
        size_ -= 1;
    }

    void Clear() {
        size_ = 0;
    }

    void Reserve(size_t cap) {
        if (cap <= cap_) {
            return;
        }
        int* nw = new int[cap]();
        for (size_t i = 0; i < size_; ++i) {
            nw[i] = data_[i];
        }
        cap_ = cap;
        delete[] data_;
        data_ = nw;
    }
    Iterator begin() {
        return Iterator(0, size_, data_);
    }
    Iterator end() {
        return Iterator(size_, size_, data_);
    }

private:
    int* data_{nullptr};
    size_t size_{0};
    size_t cap_{0};
};

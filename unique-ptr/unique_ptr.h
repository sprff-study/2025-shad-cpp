#pragma once

#include <utility>

template <class T>
class UniquePtr {
public:
    UniquePtr() {
    }
    UniquePtr(T* ptr) : ptr_(ptr) {
    }

    UniquePtr& operator=(const UniquePtr& a) = delete;
    UniquePtr& operator=(UniquePtr&& a) {
        if (&a != this) {
            delete ptr_;
            ptr_ = a.ptr_;
            a.ptr_ = nullptr;
        }
        return *this;
    };

    template <class U, std::enable_if_t<std::is_convertible_v<U*, T*>, int> = 0>
    UniquePtr& operator=(UniquePtr<U>&& a) {
        Reset(a.Release());
        return *this;
    }

    UniquePtr(const UniquePtr& a) = delete;
    template <class U, std::enable_if_t<std::is_convertible_v<U*, T*>, int> = 0>
    UniquePtr(UniquePtr<U>&& a) {
        Reset(a.Release());
    };

    ~UniquePtr() {
        delete ptr_;
    }

    T* Get() const {
        return ptr_;
    }

    T* Release() {
        auto t = ptr_;
        ptr_ = nullptr;
        return t;
    }

    void Reset() {
        delete ptr_;
        ptr_ = nullptr;
    }

    void Reset(T* nw) {
        delete ptr_;
        ptr_ = nw;
    }

    T& operator*() {
        return *ptr_;
    }

    const T& operator*() const {
        return *ptr_;
    }

    T* operator->() const {
        return ptr_;
    }

    operator bool() const {
        return ptr_ != nullptr;
    }

private:
    T* ptr_{nullptr};
};

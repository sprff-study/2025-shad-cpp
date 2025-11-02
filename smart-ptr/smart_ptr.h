#pragma once
#include <string>
struct State {
    int shared;
    int weak;
};

class WeakPtr;

class SharedPtr {
public:
    SharedPtr() {
        state_ = new State{.shared = 1, .weak = 0};
    }

    SharedPtr(std::string* p) {
        ptr_ = p;
        state_ = new State{.shared = 1, .weak = 0};
    }

    SharedPtr& operator=(const SharedPtr& other) {
        if (this == &other) {
            return *this;
        }
        DeleteState();
        ptr_ = other.ptr_;
        state_ = other.state_;
        state_->shared += 1;
        return *this;
    }

    SharedPtr& operator=(SharedPtr&& other) {
        if (this == &other) {
            return *this;
        }
        DeleteState();
        ptr_ = other.ptr_;
        state_ = other.state_;
        other.ptr_ = nullptr;
        other.state_ = nullptr;
        return *this;
    }

    void DeleteState() {
        if (state_ == nullptr) {
            return;
        }
        state_->shared -= 1;
        if (state_->shared == 0) {
            delete ptr_;
            ptr_ = nullptr;
            if (state_->weak == 0) {
                delete state_;
                state_ = nullptr;
            }
        }
    }

    SharedPtr(const SharedPtr& other) {
        ptr_ = other.ptr_;
        state_ = other.state_;
        state_->shared += 1;
    }

    SharedPtr(SharedPtr&& other) {
        ptr_ = other.ptr_;
        state_ = other.state_;
        other.ptr_ = nullptr;
        other.state_ = nullptr;
    }

    ~SharedPtr() {
        DeleteState();
    }

    std::string* Get() const {
        return ptr_;
    }

    std::string& operator*() const {
        return *ptr_;
    }

    std::string* operator->() const {
        return ptr_;
    }

    void Reset(std::string* nw = nullptr) {
        DeleteState();
        ptr_ = nw;
        state_ = new State{.shared = 1, .weak = 0};
    }

    SharedPtr(const WeakPtr& ptr);

    State* GetState() {
        return state_;
    }

private:
    std::string* ptr_{nullptr};
    State* state_{nullptr};
};

class WeakPtr {
public:
    WeakPtr() {
        state_ = new State{.shared = 0, .weak = 1};
    }

    WeakPtr(SharedPtr p) {
        ptr_ = p.Get();
        state_ = p.GetState();
        state_->weak += 1;
    }

    WeakPtr& operator=(const WeakPtr& other) {
        if (this == &other) {
            return *this;
        }
        DeleteState();
        ptr_ = other.ptr_;
        state_ = other.state_;
        state_->weak += 1;
        return *this;
    }

    WeakPtr& operator=(WeakPtr&& other) {
        if (this == &other) {
            return *this;
        }
        DeleteState();
        ptr_ = other.ptr_;
        state_ = other.state_;
        other.ptr_ = nullptr;
        other.state_ = nullptr;
        return *this;
    }

    void DeleteState() {
        if (state_ == nullptr) {
            return;
        }
        state_->weak -= 1;
        if (state_->shared == 0 && state_->weak == 0) {
            delete state_;
            state_ = nullptr;
        }
    }
    WeakPtr(const WeakPtr& other) {
        ptr_ = other.ptr_;
        state_ = other.state_;
        state_->weak += 1;
    }

    WeakPtr(WeakPtr&& other) {
        ptr_ = other.ptr_;
        state_ = other.state_;
        other.ptr_ = nullptr;
        other.state_ = nullptr;
    }

    ~WeakPtr() {
        DeleteState();
    }

    std::string* Get() const {
        if (IsExpired()) {
            return nullptr;
        }
        return ptr_;
    }

    State* GetState() const {
        return state_;
    }

    SharedPtr Lock() const {
        return SharedPtr(*this);
    }

    std::string& operator*() const {
        return *ptr_;
    }

    std::string* operator->() const {
        return ptr_;
    }

    bool IsExpired() const {
        return state_->shared == 0;
    }

private:
    std::string* ptr_{nullptr};
    State* state_{nullptr};
};

SharedPtr::SharedPtr(const WeakPtr& ptr) {
    ptr_ = ptr.Get();
    state_ = ptr.GetState();
    state_->shared += 1;
}

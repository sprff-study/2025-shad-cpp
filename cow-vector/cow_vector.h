#pragma once

#include <string>
#include <cstddef>
#include <vector>

struct State {
    size_t ref_count;  // сколько векторов делят этот State между собой.

    std::vector<std::string> data{};  // сами данные
};

class COWVector {
public:
    COWVector() {
        state_ = new State{.ref_count = 1};
    }
    ~COWVector() {
        DeleteState();
    }

    COWVector(const COWVector& other) : state_(other.state_) {
        state_->ref_count += 1;
    }

    COWVector& operator=(const COWVector& other) {
        if (this == &other) {
            return *this;
        }
        DeleteState();
        state_ = other.state_;
        state_->ref_count += 1;
        return *this;
    }

    size_t Size() const {
        return state_->data.size();
    }

    void Resize(size_t size) {
        if (size == state_->data.size()) {
            return;
        }
        DeepCopy();
        state_->data.resize(size);
    }

    const std::string& Get(size_t at) const {
        return state_->data.at(at);
    }

    const std::string& Back() const {
        return state_->data.back();
    }

    void PushBack(const std::string& value) {
        DeepCopy();
        return state_->data.push_back(value);
    }

    void Set(size_t at, const std::string& value) {
        DeepCopy();
        state_->data[at] = value;
    }

private:
    State* state_;

    void DeepCopy() {
        if (state_->ref_count == 1) {  // only owner
            return;
        }
        auto new_state = new State{1, state_->data};
        state_->ref_count -= 1;
        state_ = new_state;
    }
    void DeleteState() {
        if (state_->ref_count == 1) {
            delete state_;
        } else {
            state_->ref_count -= 1;
        }
    }
};

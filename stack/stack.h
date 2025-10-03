#pragma once

#include <cstddef>
#include <vector>

class Stack {
public:
    void Push(int x) {
        a_.push_back(x);
    }

    bool Pop() {
        if (Empty()) {
            return false;
        }
        a_.pop_back();
        return true;
    }

    int Top() const {
        return a_.back();
    }

    bool Empty() const {
        return a_.empty();
    }

    size_t Size() const {
        return a_.size();
    }

private:
    std::vector<int> a_;
};

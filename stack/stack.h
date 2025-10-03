#pragma once

#include <cstddef>
#include <vector>

class Stack {
public:
    void Push(int x) {
        a.push_back(x);
    }

    bool Pop() {
        if (Empty()) {
            return false;
        }
        a.pop_back();
        return true;
    }

    int Top() const {
        return a.back();
    }

    bool Empty() const {
        return a.empty();
    }

    size_t Size() const {
        return a.size();
    }

private:
    std::vector<int> a;
};

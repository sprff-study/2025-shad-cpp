#pragma once

#include <memory>
#include <string>

class Object : public std::enable_shared_from_this<Object> {
public:
    virtual ~Object() = default;
};

class Number : public Object {
public:
    Number(int x) : num_(x) {
    }

    int GetValue() const {
        return num_;
    }

private:
    int num_;
};

class Symbol : public Object {
public:
    Symbol(std::string s) : name_(std::move(s)) {
    }
    const std::string& GetName() const {
        return name_;
    }

private:
    std::string name_;
};

class Quote : public Object {
public:
    Quote(std::string s) : data_(std::move(s)) {
    }
    const std::string& GetString() const {
        return data_;
    }

private:
    std::string data_;
};

class Cell : public Object {
public:
    Cell(std::shared_ptr<Object> a, std::shared_ptr<Object> b) : first_(a), second_(b) {
    }
    std::shared_ptr<Object> GetFirst() const {
        return first_;
    }
    std::shared_ptr<Object> GetSecond() const {
        return second_;
    }

    void SetFirst(std::shared_ptr<Object> val) {
        first_ = val;
    }
    void SetSecond(std::shared_ptr<Object> val) {
        second_ = val;
    }

private:
    std::shared_ptr<Object> first_;
    std::shared_ptr<Object> second_;
};

template <class T>
std::shared_ptr<T> As(const std::shared_ptr<Object>& obj) {
    return std::dynamic_pointer_cast<T>(obj);
}

template <class T>
bool Is(const std::shared_ptr<Object>& obj) {
    return std::dynamic_pointer_cast<T>(obj) != nullptr;
}

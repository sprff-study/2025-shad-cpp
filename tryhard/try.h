#pragma once

#include <stdexcept>
#include <system_error>
#include <utility>
#include <exception>
#include <type_traits>
#include <variant>

namespace tag {

inline constexpr struct Exception {
} kEx;

}  // namespace tag

template <class T>
class Try {
public:
    Try() {
    }
    Try(const T& val) : data_(std::in_place_index<1>, val) {
    }

    Try(T&& val) : data_(std::in_place_index<1>, std::move(val)) {
    }

    template <typename E>
    Try(tag::Exception _, const E& val)
        : data_(std::in_place_index<2>, std::make_exception_ptr(val)) {
    }
    template <typename E>
    Try(tag::Exception _, E&& val)
        : data_(std::in_place_index<2>, std::make_exception_ptr(std::forward<E>(val))) {
    }

    Try(tag::Exception _, std::exception_ptr ptr) : data_(std::in_place_index<2>, ptr) {
    }

    Try(const Try<T>& _) = delete;
    Try(Try<T>&& _) = delete;
    Try& operator=(const Try<T>& _) = delete;
    Try& operator=(Try<T>&& _) = delete;

    const T& Value() const {
        if (data_.index() == 0) {
            throw std::logic_error("Object is empty");
        } else if (data_.index() == 1) {
            return std::get<1>(data_);
        } else {
            std::rethrow_exception(std::get<2>(data_));
        }
    }

    void Throw() const {
        if (!IsFailed()) {
            throw std::logic_error("No exception");
        }
        std::rethrow_exception(std::get<2>(data_));
    }

    bool IsFailed() const {
        return data_.index() == 2;
    }

private:
    std::variant<std::monostate, T, std::exception_ptr> data_;
};

template <>
class Try<void> {
public:
    Try() {
    }

    template <typename E>
    Try(tag::Exception _, const E& val) : data_(std::make_exception_ptr(val)) {
    }
    template <typename E>
    Try(tag::Exception _, E&& val) : data_(std::make_exception_ptr(std::forward<E>(val))) {
    }

    Try(tag::Exception _, std::exception_ptr ptr) : data_(ptr) {
    }

    Try(const Try& _) = delete;
    Try(Try&& _) = delete;
    Try& operator=(const Try& _) = delete;
    Try& operator=(Try&& _) = delete;

    void Throw() const {
        if (!IsFailed()) {
            throw std::logic_error("No exception");
        }
        std::rethrow_exception(std::get<1>(data_));
    }

    bool IsFailed() const {
        return data_.index() == 1;
    }

private:
    std::variant<std::monostate, std::exception_ptr> data_;
};

template <class Function, class... Args>
auto TryRun(Function func, Args&&... args) {
    using ReturnType = decltype(func(std::forward<Args>(args)...));
    try {
        if constexpr (std::is_void_v<ReturnType>) {
            func(std::forward<Args>(args)...);
            return Try<void>();
        } else {
            return Try<ReturnType>(func(std::forward<Args>(args)...));
        }
    } catch (int& e) {
        return Try<ReturnType>(tag::kEx, std::system_error(e, std::generic_category()));
    } catch (const char* e) {
        return Try<ReturnType>(tag::kEx, std::runtime_error(e));
    } catch (const std::exception& e) {
        return Try<ReturnType>(tag::kEx, std::current_exception());
    } catch (...) {
        return Try<ReturnType>(tag::kEx, std::logic_error("Unknown exception"));
    }
}

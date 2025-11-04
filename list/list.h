#pragma once

#include <cstddef>
#include <iterator>

template <class T>
struct Node {
    T value;
    Node<T>* next{nullptr};
    Node<T>* prev{nullptr};
};

template <class T>
class List {
public:
    class Iterator {
        friend class List;

    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        Iterator() : cur_(nullptr) {
        }
        Iterator(Node<T>* cur, Node<T>* tail = nullptr) : cur_(cur), tail_(tail) {
        }

        Iterator& operator++() {
            cur_ = cur_->next;
            return *this;
        }
        Iterator operator++(int) {
            auto ret = *this;
            cur_ = cur_->next;
            return ret;
        }

        Iterator& operator--() {
            if (cur_ == nullptr) {
                cur_ = tail_;
                return *this;
            }
            cur_ = cur_->prev;
            return *this;
        }
        Iterator operator--(int) {
            auto ret = *this;

            if (cur_ == nullptr) {
                cur_ = tail_;
                return ret;
            }
            cur_ = cur_->prev;
            return ret;
        }

        T& operator*() const {
            return cur_->value;
        }
        T* operator->() const {
            return &cur_->value;
        }

        bool operator==(const Iterator& other) const {
            return cur_ == other.cur_;
        }
        bool operator!=(const Iterator& other) const {
            return cur_ != other.cur_;
        }

    private:
        Node<T>* cur_;
        Node<T>* tail_;
    };

    List() : head_(nullptr), tail_(nullptr) {
    }

    List(const List& other) {
        for (auto x : other) {
            PushBack(x);
        }
    }
    List(List&& other) {
        head_ = other.head_;
        tail_ = other.tail_;
        other.head_ = nullptr;
        other.tail_ = nullptr;
    }
    ~List() {
        while (!IsEmpty()) {
            PopBack();
        }
    }

    List& operator=(const List& other) {
        if (this == &other) {
            return *this;
        }
        while (!IsEmpty()) {
            PopBack();
        }
        for (auto x : other) {
            PushBack(x);
        }
        return *this;
    }
    List& operator=(List&& other) {
        if (this == &other) {
            return *this;
        }
        while (!IsEmpty()) {
            PopBack();
        }
        head_ = other.head_;
        tail_ = other.tail_;
        other.head_ = nullptr;
        other.tail_ = nullptr;
        return *this;
    }

    bool IsEmpty() const {
        return head_ == nullptr;
    }
    size_t Size() const {
        Node<T>* now = head_;
        int cnt = 0;
        while (now != nullptr) {
            now = now->next;
            cnt += 1;
        }
        return cnt;
    }

    void PushBack(Node<T>* nw) {
        nw->prev = nullptr;
        nw->next = nullptr;
        if (head_ == nullptr) {
            head_ = nw;
            tail_ = nw;
            return;
        }

        tail_->next = nw;
        nw->prev = tail_;
        tail_ = nw;
    }
    void PushFront(Node<T>* nw) {
        nw->prev = nullptr;
        nw->next = nullptr;
        if (head_ == nullptr) {
            head_ = nw;
            tail_ = nw;
            return;
        }

        head_->prev = nw;
        nw->next = head_;
        head_ = nw;
    }
    void PushBack(const T& v) {
        PushBack(new Node(std::move(v)));
    }
    void PushBack(T&& v) {
        PushBack(new Node(std::move(v)));
    }
    void PushFront(const T& v) {
        PushFront(new Node(std::move(v)));
    }
    void PushFront(T&& v) {
        PushFront(new Node(std::move(v)));
    }

    T& Front() {
        return head_->value;
    }
    const T& Front() const {
        return head_->value;
    }
    T& Back() {
        return tail_->value;
    }
    const T& Back() const {
        return tail_->value;
    }

    void PopBack() {
        if (tail_->prev == nullptr) {
            delete head_;
            head_ = nullptr;
            tail_ = nullptr;
            return;
        }
        auto prev = tail_->prev;
        delete tail_;
        prev->next = nullptr;
        tail_ = prev;
    }
    void PopFront() {
        if (head_->next == nullptr) {
            delete head_;
            head_ = nullptr;
            tail_ = nullptr;
            return;
        }
        auto next = head_->next;
        delete head_;
        next->prev = nullptr;
        head_ = next;
    }

    void Erase(Iterator it) {
        Iterator prev = it;
        Iterator next = it;
        ++next;
        --prev;
        if (prev.cur_ == nullptr) {  // delete head
            PopFront();
            return;
        }
        if (next.cur_ == nullptr) {
            PopBack();
            return;
        }
        delete it.cur_;
        prev.cur_->next = next.cur_;
        next.cur_->prev = prev.cur_;
    }

    Iterator Begin() {
        return Iterator(head_, tail_);
    }
    Iterator End() {
        return Iterator(nullptr, tail_);
    }

    Iterator Begin() const {
        return Iterator(head_, tail_);
    }
    Iterator End() const {
        return Iterator(nullptr, tail_);
    }

private:
    Node<T>* head_;
    Node<T>* tail_;
};

template <class T>
List<T>::Iterator begin(List<T>& a) {
    return a.Begin();
}

template <class T>
List<T>::Iterator end(List<T>& a) {
    return a.End();
}

template <class T>
typename List<T>::Iterator begin(const List<T>& a) {
    return a.Begin();
}

template <class T>
typename List<T>::Iterator end(const List<T>& a) {
    return a.End();
}

#pragma once

#include <stdint.h>

#include "abort.h"
#include "forward.h"
#include "object_allocator.h"

namespace shstrailer {
//
// This is a limited functionality vector implementation that is designed to be
// used in an embedded environment. It has a fixed size capacity but has a
// dynamic size that can go up to the fixed size. Erasing can only be done
// through pop_back() and clear().
//
template <typename T, const uint16_t N = 16>
class Vector {
   public:
    using value_type = T;
    using reference = value_type&;
    using const_reference = const value_type&;

    template <typename I>
    class iterator {
       public:
        iterator(I** ptr) : ptr_(ptr) {}

        iterator& operator++() {
            ++ptr_;
            return *this;
        }

        [[nodiscard]] iterator operator++(int) {
            auto tmp = *this;
            ++(*this);
            return tmp;
        }

        [[nodiscard]] bool operator==(const iterator& other) const {
            return ptr_ == other.ptr_;
        }

        [[nodiscard]] bool operator!=(const iterator& other) const {
            return !(*this == other);
        }

        [[nodiscard]] I& operator*() const { return **ptr_; }

       private:
        I** ptr_;
    };

    template <typename I>
    class const_iterator {
       public:
        const_iterator(I* const* ptr) : ptr_(ptr) {}

        const_iterator& operator++() {
            ++ptr_;
            return *this;
        }

        [[nodiscard]] const_iterator operator++(int) {
            auto tmp = *this;
            ++(*this);
            return tmp;
        }

        [[nodiscard]] bool operator==(const const_iterator& other) const {
            return ptr_ == other.ptr_;
        }

        [[nodiscard]] bool operator!=(const const_iterator& other) const {
            return !(*this == other);
        }

        [[nodiscard]] const I& operator*() const { return **ptr_; }

       private:
        I* const* ptr_;
    };

    Vector() = default;
    Vector(const Vector&) = delete;
    Vector(Vector&&) = delete;
    Vector& operator=(const Vector&) = delete;
    Vector& operator=(Vector&&) = delete;

    ~Vector() { clear(); }

    template <typename... Args>
    void emplace_back(Args&&... args) {
        AbortIfOverflow();

        data_[size_] = allocator_.allocate(forward<Args>(args)...);
        ++size_;
    }

    void emplace_back(T&& value) {
        AbortIfOverflow();

        data_[size_] = allocator_.allocate(forward<T>(value));
        ++size_;
    }

    void push_back(const T& value) {
        AbortIfOverflow();

        data_[size_] = allocator_.allocate(value);
        ++size_;
    }

    void push_back(T&& value) {
        AbortIfOverflow();

        data_[size_] = allocator_.allocate(forward<T>(value));
        ++size_;
    }

    [[nodiscard]] const_reference at(const uint16_t index) const {
        if (index >= size_) {
            Abort(F("Vector index out of bounds"));
        }

        return *data_[index];
    }

    [[nodiscard]] reference at(const uint16_t index) {
        return const_cast<T&>(static_cast<const Vector*>(this)->at(index));
    }

    const_reference operator[](const uint16_t index) const {
        return *data_[index];
    }

    reference operator[](const uint16_t index) {
        return const_cast<T&>(
            static_cast<const Vector*>(this)->operator[](index));
    }

    [[nodiscard]] uint16_t size() const { return size_; }

    [[nodiscard]] uint16_t max_size() const { return N; }

    [[nodiscard]] uint16_t capacity() const { return N; }

    [[nodiscard]] bool empty() const { return 0 == size_; }

    [[nodiscard]] const_iterator<const T> begin() const {
        return const_iterator<const T>(data_);
    }

    [[nodiscard]] const_iterator<const T> end() const {
        return const_iterator<const T>(data_ + size_);
    }

    [[nodiscard]] iterator<T> begin() { return iterator<T>(data_); }

    [[nodiscard]] iterator<T> end() { return iterator<T>(data_ + size_); }

    [[nodiscard]] const_iterator<const T> cbegin() const { return begin(); }

    [[nodiscard]] const_iterator<const T> cend() const { return end(); }

    void clear() {
        for (auto& item : *this) {
            item.~T();
        }

        size_ = 0;
        allocator_.clear();
    }

    [[nodiscard]] const_reference back() const { return *data_[size_ - 1]; }

    [[nodiscard]] reference back() {
        return const_cast<T&>(static_cast<const Vector*>(this)->back());
    }

    void pop_back() {
        back().~T();
        --size_;
        allocator_.pop_back();
    }

    [[nodiscard]] const_reference front() const { return *data_[0]; }

    [[nodiscard]] reference front() {
        return const_cast<T&>(static_cast<const Vector*>(this)->front());
    }

    [[nodiscard]] const T* data() const { return data_[0]; }

    [[nodiscard]] T* data() {
        return const_cast<T*>(static_cast<const Vector*>(this)->data());
    }

   private:
    void AbortIfOverflow() {
        if (N == size_) {
            Abort(F("Vector capacity exceeded"));
        }
    }

    ObjectAllocator<T, N> allocator_;
    T* data_[N];
    uint16_t size_ = 0;
};

}  // namespace shstrailer

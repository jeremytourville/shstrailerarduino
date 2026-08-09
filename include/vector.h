#pragma once

#include <Arduino.h>

#include "abort.h"

namespace shstrailer {

//
// The N template parameter is the maximum number of elements that can be stored
// in the vector, and also can contribute to RAM size on arduino.
//
template <typename T, const uint16_t N = 16>
class Vector {
   public:
    [[nodiscard]] T& at(const size_t index) {
        return const_cast<T&>(static_cast<const Vector*>(this)->at(index));
    }

    [[nodiscard]] const T& at(const size_t index) const {
        if (index >= size_) {
            Abort(F("Vector index out of bounds"));
        }

        return data_[index];
    }

    [[nodiscard]] T& operator[](const size_t index) {
        return const_cast<T&>(
            static_cast<const Vector*>(this)->operator[](index));
    }

    [[nodiscard]] const T& operator[](const size_t index) const {
        return data_[index];
    }

    [[nodiscard]] bool empty() const { return size_ == 0; }

    [[nodiscard]] uint16_t capacity() const { return N; }

    [[nodiscard]] uint16_t size() const { return size_; }

    [[nodiscard]] T* begin() { return data_; }

    [[nodiscard]] T* end() { return data_ + size_; }

    [[nodiscard]] const T* begin() const { return data_; }

    [[nodiscard]] const T* end() const { return data_ + size_; }

    void push_back(const T& value) {
        AbortIfOverflow();

        data_[size_++] = value;
    }

    void push_back(T&& value) {
        AbortIfOverflow();

        data_[size_++] = static_cast<T&&>(value);
    }

   private:
    void AbortIfOverflow() {
        if (N == size_) {
            Abort(F("Vector overflow"));
        }
    }

    T data_[N];
    uint16_t size_ = 0;
};

}  // namespace shstrailer

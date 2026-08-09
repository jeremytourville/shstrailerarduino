#pragma once

#include <stddef.h>
#include <stdint.h>

#include "abort.h"

namespace shstrailer {

template <typename T, const uint16_t N>
class Array {
   public:
    // must be public to allow aggregate initialization
    T data_[N];

    [[nodiscard]] T& at(const size_t index) {
        return const_cast<T&>(static_cast<const Array*>(this)->at(index));
    }

    [[nodiscard]] const T& at(const size_t index) const {
        if (index >= N) {
            Abort(F("Array index out of bounds"));
        }

        return data_[index];
    }

    [[nodiscard]] T& operator[](const size_t index) {
        return const_cast<T&>(
            static_cast<const Array*>(this)->operator[](index));
    }

    [[nodiscard]] const T& operator[](const size_t index) const {
        return data_[index];
    }

    [[nodiscard]] constexpr bool empty() const { return N == 0; }

    [[nodiscard]] constexpr uint16_t capacity() const { return N; }

    [[nodiscard]] constexpr uint16_t size() const { return N; }

    [[nodiscard]] T* begin() { return data_; }

    [[nodiscard]] T* end() { return data_ + N; }

    [[nodiscard]] const T* begin() const { return data_; }

    [[nodiscard]] const T* end() const { return data_ + N; }
};

}  // namespace shstrailer

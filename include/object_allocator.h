#pragma once

#include <Arduino.h>

#include <new>

#include "abort.h"
#include "forward.h"

namespace shstrailer {

template <typename T, const uint8_t N>
class ObjectAllocator {
   public:
    template <typename... Args>
    [[nodiscard]] T* allocate(Args&&... args) {
        AbortIfOverflow();

        T* obj =
            new (buffer_[allocatedCount_].storage) T(forward<Args>(args)...);
        ++allocatedCount_;

        return obj;
    }

    [[nodiscard]] T* allocate(T&& value) {
        AbortIfOverflow();

        T* obj = new (buffer_[allocatedCount_].storage) T(forward<T>(value));
        ++allocatedCount_;

        return obj;
    }

    void clear() { allocatedCount_ = 0; }

    void pop_back() { --allocatedCount_; }

   private:
    void AbortIfOverflow() {
        if (allocatedCount_ >= N) {
            Abort(F("ObjectAllocator overflow"));
        }
    }

    struct Slot {
        alignas(T) uint8_t storage[sizeof(T)];
    };

    Slot buffer_[N];
    size_t allocatedCount_ = 0;
};

}  // namespace shstrailer

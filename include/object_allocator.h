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
    T* allocate(Args&&... args) {
        if (allocatedCount_ >= N) {
            Abort(F("ObjectAllocator overflow"));
        }

        T* obj = new (buffer_ + allocatedCount_ * sizeof(T))
            T(forward<Args>(args)...);
        ++allocatedCount_;
        return obj;
    }

   private:
    alignas(T) uint8_t buffer_[sizeof(T) * N];
    size_t allocatedCount_ = 0;
};

}  // namespace shstrailer
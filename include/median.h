#pragma once

#include <stdint.h>

namespace shstrailer {

void swap(uint32_t& lhs, uint32_t& rhs);

[[nodiscard]] uint32_t median(uint32_t lhs, uint32_t middle, uint32_t rhs);

template <const uint8_t kAverageSamples>
void medianFilter(uint32_t (&output)[kAverageSamples],
                  const uint32_t (&input)[kAverageSamples]) {
    // this only works for powers of two
    static_assert((kAverageSamples & (kAverageSamples - 1)) == 0);

    constexpr uint8_t kMask = kAverageSamples - 1;

    // Treat input/output as ring buffers. A mask can be used instead of modulus
    // since which requires power of two sizes. Modulus is slower on arduinos.
    for (uint8_t i = kAverageSamples - 1; i < 2 * kAverageSamples - 1; ++i) {
        output[(i + 1) & kMask] = median(
            input[i & kMask], input[(i + 1) & kMask], input[(i + 2) & kMask]);
    }
}

}  // namespace shstrailer

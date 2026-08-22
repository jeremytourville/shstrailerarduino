#include "median.h"

namespace shstrailer {

void swap(uint32_t& lhs, uint32_t& rhs) {
    const auto tmp = rhs;
    rhs = lhs;
    lhs = tmp;
}

[[nodiscard]] uint32_t median(uint32_t lhs, uint32_t middle, uint32_t rhs) {
    if (lhs > middle) {
        swap(lhs, middle);
    }

    if (middle > rhs) {
        swap(middle, rhs);
    }

    if (lhs > middle) {
        swap(lhs, middle);
    }

    return middle;
}

}  // namespace shstrailer

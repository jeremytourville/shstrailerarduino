#pragma once

namespace shstrailer {

template <typename T>
T minimum(const T& a, const T& b) {
    return (a < b) ? a : b;
}

template <typename T>
T maximum(const T& a, const T& b) {
    return (a > b) ? a : b;
}

template <typename T>
T clamp(const T& value, const T& low, const T& high) {
    return minimum(maximum(value, low), high);
}

}  // namespace shstrailer

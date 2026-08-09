#pragma once

namespace shstrailer {

template <typename T>
struct remove_reference {
    typedef T type;
};

template <typename T>
struct remove_reference<T&> {
    typedef T type;
};

template <typename T>
struct remove_reference<T&&> {
    typedef T type;
};

template <typename T>
inline constexpr T&& forward(
    typename remove_reference<T>::type& type) noexcept {
    return static_cast<T&&>(type);
}

template <typename T>
inline constexpr T&& forward(
    typename remove_reference<T>::type&& type) noexcept {
    return static_cast<T&&>(type);
}

}  // namespace shstrailer

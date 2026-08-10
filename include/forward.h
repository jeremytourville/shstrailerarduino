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

template <typename T>
constexpr typename remove_reference<T>::type&& move(T&& arg) noexcept {
    return static_cast<typename remove_reference<T>::type&&>(arg);
}

}  // namespace shstrailer

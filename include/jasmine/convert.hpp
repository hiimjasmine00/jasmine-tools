#pragma once
#include <fast_float/fast_float.h>

namespace jasmine::convert {
    template <class T>
    bool to(std::string_view str, T& out) {
        return fast_float::from_chars(str.data(), str.data() + str.size(), out).ec == std::errc();
    }

    template <class T>
    std::optional<T> get(std::string_view str) {
        T value = {};
        return to(str, value) ? std::optional<T>(value) : std::nullopt;
    }

    template <class T>
    T getOr(std::string_view str, T defaultValue = {}) {
        T value;
        return to(str, value) ? value : defaultValue;
    }
}

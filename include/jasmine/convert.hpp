#pragma once
#ifdef __cpp_lib_to_chars
#include <charconv>
#else
#include <Geode/utils/general.hpp>
#endif

namespace jasmine::convert {
    template <std::integral T>
    bool toInt(std::string_view str, T& out) {
        return std::from_chars(str.data(), str.data() + str.size(), out).ec == std::errc();
    }

    template <std::integral T>
    std::optional<T> getInt(std::string_view str) {
        T value = {};
        return toInt(str, value) ? std::optional<T>(value) : std::nullopt;
    }

    template <std::floating_point T>
    bool toFloat(std::string_view str, T& out) {
        #ifdef __cpp_lib_to_chars
        return std::from_chars(str.data(), str.data() + str.size(), out).ec == std::errc();
        #else
        if (geode::Result<T> res = geode::utils::numFromString<T>(str)) {
            out = res.unwrap();
            return true;
        }
        return false;
        #endif
    }

    template <std::floating_point T>
    std::optional<T> getFloat(std::string_view str) {
        T value = {};
        return toFloat(str, value) ? std::optional<T>(value) : std::nullopt;
    }
}

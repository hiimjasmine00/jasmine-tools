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
    std::pair<T, bool> getInt(std::string_view str) {
        T value = {};
        return std::make_pair(value, toInt(str, value));
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
    std::pair<T, bool> getFloat(std::string_view str) {
        T value = {};
        return std::make_pair(value, toFloat(str, value));
    }
}

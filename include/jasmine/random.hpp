#pragma once
#include <random>

namespace jasmine::random {
    double get();
    bool getBool();

    template <std::integral T>
    T getInt(T min, T max) {
        static std::mt19937 mt(std::random_device{}());
        return std::uniform_int_distribution<T>(min, max)(mt);
    }
}

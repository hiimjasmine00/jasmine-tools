#pragma once
#include <Geode/platform/cplatform.h>
#include <string_view>
#include <vector>

#ifdef GEODE_IS_ANDROID
#define JASMINE_STRING(...) std::string(__VA_ARGS__)
#else
#define JASMINE_STRING(...) __VA_ARGS__
#endif

namespace jasmine::string {
    std::vector<std::string_view> split(std::string_view str, char delimiter);
    std::vector<std::string_view> split(std::string_view str, std::string_view delimiter);
}

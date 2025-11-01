#pragma once
#include <string_view>
#include <vector>

namespace jasmine::string {
    std::vector<std::string_view> split(std::string_view str, char delimiter);
    std::vector<std::string_view> split(std::string_view str, std::string_view delimiter);
}

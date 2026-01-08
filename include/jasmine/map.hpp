#include <string>
#include <unordered_map>

namespace jasmine::map {
    struct StringHash {
        using is_transparent = void;

        size_t operator()(const char* str) const {
            return std::hash<std::string_view>()(str);
        }

        size_t operator()(std::string_view str) const {
            return std::hash<std::string_view>()(str);
        }

        size_t operator()(const std::string& str) const {
            return std::hash<std::string_view>()(str);
        }
    };

    template <class T>
    using StringMap = std::unordered_map<std::string, T, StringHash, std::equal_to<>>;
}

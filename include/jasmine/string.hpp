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
    struct SplitCharIterator {
        std::string_view str;
        char delimiter;
        size_t pos = 0;

        SplitCharIterator(std::string_view str, char delimiter) : str(str), delimiter(delimiter), pos(0) {}

        bool operator==(const SplitCharIterator& other) const {
            return pos == other.pos;
        }

        bool operator!=(const SplitCharIterator& other) const {
            return pos != other.pos;
        }

        std::string_view operator*() const {
            auto end = str.find(delimiter, pos);
            return str.substr(pos, end - (end == std::string_view::npos ? 0 : pos));
        }

        SplitCharIterator& operator++() {
            auto end = str.find(delimiter, pos);
            if (end == std::string_view::npos) pos = str.size();
            else pos = end + 1;
            return *this;
        }
    };

    inline SplitCharIterator begin(SplitCharIterator it) {
        return it;
    }

    inline SplitCharIterator end(SplitCharIterator it) {
        it.pos = it.str.size();
        return it;
    }

    struct SplitStringIterator {
        std::string_view str;
        std::string_view delimiter;
        size_t pos = 0;

        SplitStringIterator(std::string_view str, std::string_view delimiter) : str(str), delimiter(delimiter), pos(0) {}

        bool operator==(const SplitStringIterator& other) const {
            return pos == other.pos;
        }

        bool operator!=(const SplitStringIterator& other) const {
            return pos != other.pos;
        }

        std::string_view operator*() const {
            auto end = str.find(delimiter, pos);
            return str.substr(pos, end - (end == std::string_view::npos ? 0 : pos));
        }

        SplitStringIterator& operator++() {
            auto end = str.find(delimiter, pos);
            if (end == std::string_view::npos) pos = str.size();
            else pos = end + delimiter.size();
            return *this;
        }
    };

    inline SplitStringIterator begin(SplitStringIterator it) {
        return it;
    }

    inline SplitStringIterator end(SplitStringIterator it) {
        it.pos = it.str.size();
        return it;
    }

    std::vector<std::string_view> split(std::string_view str, char delimiter);
    std::vector<std::string_view> split(std::string_view str, std::string_view delimiter);
}

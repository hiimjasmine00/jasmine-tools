#pragma once
#include <Geode/platform/cplatform.h>
#include <string_view>

#ifdef GEODE_IS_ANDROID
#define JASMINE_STRING(...) std::string(__VA_ARGS__)
#else
#define JASMINE_STRING(...) __VA_ARGS__
#endif

namespace jasmine::string {
    struct SplitIterator {
        std::string_view str;
        std::string_view delimiter;
        size_t pos = 0;

        SplitIterator(std::string_view str, std::string_view delimiter) : str(str), delimiter(delimiter), pos(0) {}

        bool operator==(const SplitIterator& other) const {
            return pos == other.pos;
        }

        bool operator!=(const SplitIterator& other) const {
            return pos != other.pos;
        }

        std::string_view operator*() const {
            auto end = str.find(delimiter, pos);
            return str.substr(pos, end - (end == std::string_view::npos ? 0 : pos));
        }

        SplitIterator& operator++() {
            auto end = str.find(delimiter, pos);
            if (end == std::string_view::npos) pos = str.size();
            else pos = end + delimiter.size();
            return *this;
        }
    };

    inline SplitIterator begin(SplitIterator it) {
        return it;
    }

    inline SplitIterator end(SplitIterator it) {
        it.pos = it.str.size();
        return it;
    }
}

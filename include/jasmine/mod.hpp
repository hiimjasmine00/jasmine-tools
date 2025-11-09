#pragma once
#include <cstddef>

namespace jasmine::mod {
    namespace internal {
        template <size_t N>
        struct StringConcatModIDSlash {
            static constexpr size_t extra = sizeof(GEODE_MOD_ID);
            char buffer[extra + N]{};
            constexpr StringConcatModIDSlash(const char (&pp)[N]) {
                char id[] = GEODE_MOD_ID;
                for (size_t i = 0; i < sizeof(id); i++) {
                    buffer[i] = id[i];
                }
                buffer[extra - 1] = '/';
                for (size_t i = 0; i < N; i++) {
                    buffer[extra + i] = pp[i];
                }
            }
        };
    }

    template <internal::StringConcatModIDSlash Str>
    constexpr auto operator""_spr() {
        return Str.buffer;
    }
}

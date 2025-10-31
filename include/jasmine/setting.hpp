#pragma once
#include <Geode/loader/SettingV3.hpp>

namespace jasmine::setting {
    geode::SettingV3* getInternal(std::string_view key);

    template <class T>
    geode::SettingTypeForValueType<T>::SettingType* get(std::string_view key) {
        return static_cast<geode::SettingTypeForValueType<T>::SettingType*>(getInternal(key));
    }

    template <class T>
    T getValue(std::string_view key) {
        if (auto setting = get<T>(key)) {
            if (setting->shouldEnable()) return setting->getValue();
        }
        return {};
    }
}

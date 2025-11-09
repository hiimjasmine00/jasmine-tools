#pragma once
#include <Geode/cocos/cocoa/CCArray.h>
#include <span>

namespace jasmine::array {
    template <std::derived_from<cocos2d::CCObject> T>
    T** begin(cocos2d::CCArray* arr) {
        return reinterpret_cast<T**>(arr->data->arr);
    }

    template <std::derived_from<cocos2d::CCObject> T>
    T** end(cocos2d::CCArray* arr) {
        return reinterpret_cast<T**>(arr->data->arr) + arr->data->num;
    }

    template <std::derived_from<cocos2d::CCObject> T>
    std::span<T*> toSpan(cocos2d::CCArray* arr) {
        return std::span<T*>(reinterpret_cast<T**>(arr->data->arr), arr->data->num);
    }
}

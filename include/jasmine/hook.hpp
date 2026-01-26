#pragma once
#include <cocos2d.h>
#include <Geode/loader/Tulip.hpp>
#include <Geode/loader/Types.hpp>
#include <Geode/utils/StringMap.hpp>

namespace jasmine::hook {
    geode::Hook* createInternal(
        uintptr_t address, std::string name, void* detour,
        const tulip::hook::HandlerMetadata& metadata, geode::Function<void(geode::Hook*)> preClaim
    );

    template <class T>
    geode::Hook* create(uintptr_t address, std::string name, T detour, geode::Function<void(geode::Hook*)> preClaim = nullptr) {
        return createInternal(
            address,
            std::move(name),
            reinterpret_cast<void*>(detour),
            {
                geode::hook::createConvention(tulip::hook::TulipConvention::Default),
                tulip::hook::AbstractFunction::from(detour)
            },
            std::move(preClaim)
        );
    }

    geode::Hook* get(geode::utils::StringMap<std::shared_ptr<geode::Hook>>& hooks, std::string_view name, bool enabled);
    void modify(geode::utils::StringMap<std::shared_ptr<geode::Hook>>& hooks, std::string_view key);
    void modify(geode::utils::StringMap<std::shared_ptr<geode::Hook>>& hooks, std::string_view name, std::string_view key);
    void toggle(geode::Hook* hook, bool enable);
}

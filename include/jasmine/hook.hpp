#pragma once
#include <cocos2d.h>
#include <Geode/loader/Tulip.hpp>
#include <Geode/loader/Types.hpp>

namespace jasmine::hook {
    geode::Hook* createInternal(
        uintptr_t address, std::string_view name, void* detour,
        const tulip::hook::HandlerMetadata& metadata, std::function<void(geode::Hook*)> preClaim
    );

    template <class T>
    geode::Hook* create(uintptr_t address, std::string_view name, T detour, std::function<void(geode::Hook*)> preClaim = nullptr) {
        return createInternal(
            address,
            name,
            reinterpret_cast<void*>(detour),
            {
                geode::hook::createConvention(tulip::hook::TulipConvention::Default),
                tulip::hook::AbstractFunction::from(detour)
            },
            std::move(preClaim)
        );
    }

    geode::Hook* get(std::map<std::string, std::shared_ptr<geode::Hook>>& hooks, std::string_view name, bool enabled);
    void modify(std::map<std::string, std::shared_ptr<geode::Hook>>& hooks, std::string_view key);
    void modify(std::map<std::string, std::shared_ptr<geode::Hook>>& hooks, std::string_view name, std::string_view key);
    void toggle(geode::Hook* hook, bool enable);
}

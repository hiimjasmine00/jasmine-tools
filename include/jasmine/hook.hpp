#pragma once
#include <cocos2d.h>
#include <Geode/loader/Types.hpp>

namespace jasmine::hook {
    geode::Hook* get(std::map<std::string, std::shared_ptr<geode::Hook>>& hooks, std::string_view key, bool enabled);
    void modify(std::map<std::string, std::shared_ptr<geode::Hook>>& hooks, std::string_view key);
    void toggle(geode::Hook* hook, bool enable);
}

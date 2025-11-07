#pragma once
#include <Geode/binding/GJSearchObject.hpp>

namespace jasmine::search {
    template <std::input_iterator It, std::sentinel_for<It> Sent, class Func = std::identity>
    requires std::is_invocable_r_v<std::string, Func, typename std::iterator_traits<It>::value_type>
    GJSearchObject* getObject(It begin, Sent end, Func func = {}) {
        auto obj = GJSearchObject::create(SearchType::Type19);
        fmt::memory_buffer searchQuery;
        for (auto it = begin; it < end; ++it) {
            if (it != begin) searchQuery.push_back(',');
            fmt::format_to(std::back_inserter(searchQuery), "{}", std::invoke(func, *it));
        }
        obj->m_searchQuery = fmt::to_string(searchQuery);
        return obj;
    }

    template <std::ranges::input_range Rng, class Func = std::identity>
    requires std::is_invocable_r_v<std::string, Func, typename std::ranges::range_value_t<Rng>>
    GJSearchObject* getObject(Rng&& range, Func func = {}) {
        return getObject(std::ranges::begin(range), std::ranges::end(range), std::move(func));
    }

    const char* getKey(GJSearchObject* object);
}

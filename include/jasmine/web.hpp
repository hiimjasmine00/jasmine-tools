#pragma once
#include <Geode/utils/web.hpp>

namespace jasmine::web {
    std::vector<matjson::Value> getArray(const geode::utils::web::WebResponse& response, std::string_view key = {});
    std::string getString(const geode::utils::web::WebResponse& response);
}

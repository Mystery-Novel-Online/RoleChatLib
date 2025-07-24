#pragma once

#include <string>
#include <nlohmann/json.hpp>

namespace rolechat::JsonUtils {
    using JsonData = nlohmann::json;
    JsonData loadFile(const std::string& path, bool& validJson);
}
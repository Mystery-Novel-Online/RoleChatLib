#pragma once

#include <nlohmann/json.hpp>
#include "rolechat/util/JsonUtils.h"
#include "rolechat/background/IBackgroundData.h"

namespace rolechat::background {

using JSONObject = rolechat::JsonUtils::JsonData;

class JsonBackgroundData : public IBackgroundData {
public:
    JsonBackgroundData(){};
    void loadBackground(const std::string& backgroundPath);
    void parseBackgroundVariant(const std::string& name, const JSONObject& variantObject);
};

}

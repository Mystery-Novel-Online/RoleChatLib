#include "rolechat/background/JsonBackgroundData.h"
#include "rolechat/util/JsonUtils.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <regex>

using namespace rolechat::background;

void JsonBackgroundData::loadBackground(const std::string &backgroundPath)
{
    bool validJson = false;
    rolechat::JsonUtils::JsonData j = rolechat::JsonUtils::loadFile(backgroundPath, validJson);
    if(validJson == false) 
    {
        std::cerr << "Invalid JSON data in file: " << backgroundPath << "\n";
        return;
    }

    for (auto& [key, pos] : j["positions"].items()) 
    {
        assignPosition(key, { pos["background"].get<std::string>(), pos["foreground"].get<std::string>() });
    }

}
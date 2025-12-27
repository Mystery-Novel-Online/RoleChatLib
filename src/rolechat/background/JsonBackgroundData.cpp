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
    j.items();
    for (auto& [variantName, variantData] : j.items())
    {
      for (auto& [key, pos] : variantData["positions"].items())
      {
        assignPosition(variantName, key, { pos["background"].get<std::string>(), pos["foreground"].get<std::string>() });
      }
    }

}

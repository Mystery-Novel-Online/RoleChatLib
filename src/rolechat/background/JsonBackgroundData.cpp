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
      if(variantData.contains("background"))
      {
        std::string defaultBg = variantData.value("background", "");
        std::string defaultForeground = variantData.value("foreground", "");
        assignPosition(variantName, "default", { defaultBg, defaultForeground });
      }

      if(variantData.contains("positions") && variantData["positions"].is_array())
      {
        for (auto& [key, pos] : variantData["positions"].items())
        {
          std::string background = pos.value("background", "");
          std::string foreground = pos.value("foreground", "");
          assignPosition(variantName, key, { background, foreground });
        }
      }
    }

}

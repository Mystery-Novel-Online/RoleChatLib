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
    JSONObject j = rolechat::JsonUtils::loadFile(backgroundPath, validJson);
    if(validJson == false) 
    {
        std::cerr << "Invalid JSON data in file: " << backgroundPath << "\n";
        return;
    }
    j.items();

    if(j.contains("background"))
    {
      parseBackgroundVariant("Default", j);
    }

    for (auto& [variantName, variantData] : j.items())
    {
      parseBackgroundVariant(variantName, variantData);
    }

    if(j.value("randomize_position", false))
    {
      randomizeDefault();
    }

}

void JsonBackgroundData::parseBackgroundVariant(const std::string &name, const JSONObject &variantObject)
{
  if(variantObject.contains("background"))
  {
    std::string defaultBg = variantObject.value("background", "");
    std::string defaultForeground = variantObject.value("foreground", "");
    std::string defaultAmbient = variantObject.value("ambience", "");
    assignPosition(name, "Default", { defaultBg, defaultForeground, defaultAmbient });
  }

  if(variantObject.contains("positions") && variantObject["positions"].is_object())
  {
    for (auto& [key, pos] : variantObject["positions"].items())
    {
      std::string background = pos.value("background", "");
      std::string foreground = pos.value("foreground", "");
      std::string ambient = pos.value("foreground", "");
      assignPosition(name, key, { background, foreground, ambient });
    }
  }
}


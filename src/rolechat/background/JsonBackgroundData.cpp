#include "rolechat/background/JsonBackgroundData.h"
#include "rolechat/util/JsonUtils.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <regex>
#include <filesystem>
#include "rolechat/filesystem/RCFile.h"

using namespace rolechat::background;

void JsonBackgroundData::loadBackground(const std::string &backgroundPath)
{
    bool validJson = false;
    JSONObject j = rolechat::JsonUtils::loadFile(backgroundPath + "/background.json", validJson);
    if(validJson == false)
    {
        std::cerr << "Invalid JSON data in file: " << backgroundPath + "/background.json" << "\n";
        return;
    }
    m_backgroundPath = backgroundPath;
    j.items();

    if(j.contains("background"))
    {
      parseBackgroundVariant("Default", j);
    }

    for (auto& [variantName, variantData] : j.items())
    {
      parseBackgroundVariant(variantName, variantData);
    }

    searchVariantDirectory();

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
    std::map<std::string, BackgroundPosition> timeVariants = {};

    if(variantObject.contains("time_variants"))
    {
      for (auto& [variantName, variantData] : variantObject["time_variants"].items())
      {
        std::string timeBg = variantData.value("background", "");
        std::string timeForeground = variantData.value("foreground", "");
        std::string timeAmbient = variantData.value("ambience", "");
        timeVariants[variantName] = {timeBg, timeForeground, timeAmbient};
      }
    }

    assignPosition(name, "Default", { defaultBg, defaultForeground, defaultAmbient, timeVariants });
  }


  if(variantObject.contains("positions") && variantObject["positions"].is_object())
  {
    for (auto& [key, pos] : variantObject["positions"].items())
    {
      std::string background = pos.value("background", "");
      std::string foreground = pos.value("foreground", "");
      std::string ambient = pos.value("ambience", "");
      std::map<std::string, BackgroundPosition> timeVariants = {};

      if(pos.contains("time_variants"))
      {
        for (auto& [variantName, variantData] : pos.items())
        {
          std::string timeBg = variantData.value("background", "");
          std::string timeForeground = variantData.value("foreground", "");
          std::string timeAmbient = variantData.value("ambience", "");
          timeVariants[variantName] = {timeBg, timeForeground, timeAmbient};
        }
      }

      assignPosition(name, key, { background, foreground, ambient, timeVariants });
    }
  }
}

void JsonBackgroundData::searchVariantDirectory()
{
  std::filesystem::path variantPath = std::filesystem::u8path(m_backgroundPath + "/variants");

  std::error_code ec;

  if (!std::filesystem::is_directory(variantPath, ec)) {
    return;
  }

  std::vector<std::string> subdirs;
  for (const auto& entry : std::filesystem::directory_iterator(variantPath)) {
    if (entry.is_directory()) {
      subdirs.push_back(entry.path().filename().string());
    }
  }
  if (subdirs.empty()) return;

  for (const std::string& name : subdirs)
  {
    std::filesystem::path variantJsonPath = std::filesystem::u8path(m_backgroundPath + "/variants/" + name + "/variant.json");

    if(!fs::RCFile::exists(variantJsonPath.string()))
      continue;

    bool validJson = false;
    JSONObject j = rolechat::JsonUtils::loadFile(variantJsonPath.string(), validJson);
    if(validJson == false)
    {
      std::cerr << "Invalid JSON data in file: " << variantJsonPath.string() << "\n";
      continue;
    }

    parseBackgroundVariant(name, j);

  }

}


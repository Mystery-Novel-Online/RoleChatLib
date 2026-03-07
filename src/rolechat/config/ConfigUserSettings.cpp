#include "rolechat/config/ConfigUserSettings.h"
#include "rolechat/util/JsonUtils.h"
#include <fstream>
#include <iostream>

std::filesystem::path getConfigPath()
{
#ifdef _WIN32
  const char* appdata = std::getenv("APPDATA");
  std::filesystem::path dir = std::filesystem::path(appdata) / "Rolechat";
#else
  const char* home = std::getenv("HOME");
  std::filesystem::path dir = std::filesystem::path(home) / ".config" / "rolechat";
#endif

  std::filesystem::create_directories(dir);
  return dir / "config.json";
}

std::filesystem::path getOldConfigPath()
{
  return "base/configs/config.json";
}

using namespace rolechat::config;

std::unordered_map<std::string, std::string> ConfigUserSettings::m_configStrings = {};
std::unordered_map<std::string, bool> ConfigUserSettings::m_configBooleans = {};
std::unordered_map<std::string, int> ConfigUserSettings::m_configIntegers = {};
std::unordered_map<std::string, float> ConfigUserSettings::m_configFloats = {};

void migrateConfigIfNeeded()
{
  auto newPath = getConfigPath();
  auto oldPath = getOldConfigPath();

  if (!std::filesystem::exists(newPath) && std::filesystem::exists(oldPath))
  {
    try
    {
      std::filesystem::rename(oldPath, newPath);
    }
    catch (...)
    {
      std::filesystem::copy_file(oldPath, newPath, std::filesystem::copy_options::overwrite_existing);
      std::filesystem::remove(oldPath);
    }
  }
}

void ConfigUserSettings::save()
{
    using namespace rolechat::JsonUtils;
    JsonData configObject;

    for (const auto& [key, value] : m_configStrings) 
        configObject[key] = value;
    
    for (const auto& [key, value] : m_configBooleans) 
        configObject[key] = value;
    
    for (const auto& [key, value] : m_configIntegers) 
        configObject[key] = value;
    
    for (const auto& [key, value] : m_configFloats) 
        configObject[key] = value;

    JsonData fileObject;
    fileObject["config"] = configObject;

    std::ofstream file(getConfigPath());
    if (!file.is_open()) {
        std::cerr << "Failed to open config file for writing.\n";
        return;
    }

    file << fileObject.dump(4);
    file.close();
}

void ConfigUserSettings::load()
{
    using namespace rolechat::JsonUtils;

    migrateConfigIfNeeded();

    std::ifstream file(getConfigPath());
    if (!file.is_open()) {
        std::cerr << "Failed to open config file for reading.\n";
        return;
    }

    JsonData fileObject;
    try {
        file >> fileObject;
    } catch (const std::exception& e) {
        std::cerr << "Failed to parse JSON: " << e.what() << "\n";
        return;
    }

    const auto& configObject = fileObject["config"];

    m_configStrings.clear();
    m_configBooleans.clear();
    m_configIntegers.clear();

    for (auto it = configObject.begin(); it != configObject.end(); ++it) {
        const std::string& key = it.key();
        const auto& value = it.value();

        if (value.is_string()) {
            m_configStrings[key] = value.get<std::string>();
        } else if (value.is_boolean()) {
            m_configBooleans[key] = value.get<bool>();
        } else if (value.is_number_integer()) {
            m_configIntegers[key] = value.get<int>();
        } else if (value.is_number_float()) {
            m_configFloats[key] = value.get<float>();
        }  else {
            std::cerr << "Unknown type for config key: " << key << "\n";
        }
    }
}

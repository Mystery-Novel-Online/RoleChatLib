#include "blip_config.h"
#include "rolechat/filesystem/RCDir.h"
#include "rolechat/filesystem/RCFile.h"
#include "rolechat/util/JsonUtils.h"
#include <iostream>
#include <random>

static const std::string& pickRandom(const std::vector<std::string>& vec)
{
  static thread_local std::mt19937 rng{ std::random_device{}() };

  std::uniform_int_distribution<size_t> dist(0, vec.size() - 1);
  return vec[dist(rng)];
}

BlipConfig::BlipConfig(const std::string& name) : m_name(name)
{
  rolechat::fs::RCDir blipDirectory("sounds/blips/" + m_name);
  std::string configPath = blipDirectory.findFirst() + "/blip.json";
  bool configExists = rolechat::fs::RCFile::exists(configPath);

  rolechat::JsonUtils::JsonData jsonData = rolechat::JsonUtils::loadFile(configPath, m_validConfig);

  if(!m_validConfig)
    return;

  if(jsonData["sound"].is_string())
    m_sound.push_back(jsonData.value("sound", "default.wav"));
  else if(jsonData["sound"].is_array())
  {
    for (const auto& s : jsonData["sound"])
    {
      if (s.is_string())
      {
        m_sound.push_back(s.get<std::string>());
      }
    }
  }

  m_allowBlank = jsonData.value("allow_blanks", false);

  m_blipRate = jsonData.value("blip_rate", 30);

  if(jsonData.contains("gender_overrides") && jsonData["gender_overrides"].is_object())
  {
    for (auto& [gender, sound] : jsonData["gender_overrides"].items())
    {
      if(sound.is_string())
      {
        m_genderOverrides[gender].push_back(sound);
      }
      else if(sound.is_array())
      {
        for (const auto& s : sound)
        {
          if (s.is_string())
          {
            m_genderOverrides[gender].push_back(s.get<std::string>());
          }
        }
      }
    }
  }
}

std::string BlipConfig::soundFile(const std::string &gender)
{
  auto it = m_genderOverrides.find(gender);
  if (it != m_genderOverrides.end())
    return pickRandom(it->second);

  if (!m_sound.empty())
    return pickRandom(m_sound);

  return {};
}

#include "blip_config.h"
#include "rolechat/filesystem/RCDir.h"
#include "rolechat/filesystem/RCFile.h"
#include "rolechat/util/JsonUtils.h"

BlipConfig::BlipConfig(const std::string& name) : m_name(name)
{
  rolechat::fs::RCDir blipDirectory("sounds/blips/" + m_name);
  std::string configPath = blipDirectory.findFirst() + "/blip.json";
  bool configExists = rolechat::fs::RCFile::exists(configPath);

  rolechat::JsonUtils::JsonData jsonData = rolechat::JsonUtils::loadFile(configPath, m_validConfig);

  if(!m_validConfig)
    return;

  m_sound = jsonData.value("sound", "default.wav");
  m_allowBlank = jsonData.value("allow_blanks", false);

  m_blipRate = jsonData.value("blip_rate", 30);

  if(jsonData.contains("gender_overrides") && jsonData["gender_overrides"].is_object())
  {

    for (auto& [gender, sound] : jsonData["gender_overrides"].items())
    {
      m_genderOverrides[gender] = sound;
    }
  }
}

std::string BlipConfig::soundFile(const std::string &gender)
{
  auto it = m_genderOverrides.find(gender);
  if (it != m_genderOverrides.end()) return it->second;
  return m_sound;
}

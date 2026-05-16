#include "blip_config.h"

BlipConfig::BlipConfig(const std::string& name) : m_name(name)
{
}

std::string BlipConfig::soundFile(const std::string &gender)
{
  auto it = m_genderOverrides.find(gender);
  if (it != m_genderOverrides.end()) return it->second;
  return m_sound;
}

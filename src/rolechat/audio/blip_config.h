#ifndef BLIP_CONFIG_H
#define BLIP_CONFIG_H

#include <string>
#include <map>

class BlipConfig
{
public:
  BlipConfig(const std::string& name);

  std::string name() { return m_name; };
  std::string soundFile(const std::string& gender);
  int blipRate() { return m_blipRate; };

private:
  std::string m_name;
  std::string m_sound;
  int m_blipRate = 0;

  std::map<std::string, std::string> m_genderOverrides = {};
};

#endif // BLIP_CONFIG_H

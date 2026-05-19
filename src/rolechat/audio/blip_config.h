#ifndef BLIP_CONFIG_H
#define BLIP_CONFIG_H

#include <string>
#include <map>
#include <vector>

class BlipConfig
{
public:
  BlipConfig(const std::string& name);

  std::string name() const { return m_name; };
  std::string soundFile(const std::string& gender) const;
  int blipRate() const { return m_blipRate; };
  bool blanksAllowed() const { return m_allowBlank; }

  bool valid() const { return m_validConfig; }

private:
  std::string m_name;
  std::vector<std::string> m_sound;
  int m_blipRate = 0;
  bool m_allowBlank = false;

  bool m_validConfig = false;

  std::map<std::string, std::vector<std::string>> m_genderOverrides = {};
};

#endif // BLIP_CONFIG_H

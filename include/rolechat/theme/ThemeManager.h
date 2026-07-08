#pragma once

#include <string>
#include <memory>
#include "ThemeModule.h"
#include "ThemeGameMode.h"

namespace rolechat::theme {
class ThemeManager
{
public:
  static ThemeManager& Instance()
  {
    static ThemeManager instance;
    return instance;
  }

  bool LoadTheme(const std::string& name);
  void ReloadTheme();
  void SwitchGamemode(const std::string& gamemode);
  void SwitchTime(const std::string& time);

  void SetResizeFactor(double factor) { m_resize = factor; if(factor <= 0.0) m_resize = 0.1; };

  ThemeGameMode& GetTheme()
  {
    return *m_currentTheme;
  }

  const ThemeElement GetElement(ThemeSceneType scene, const std::string& id);


private:
  std::unique_ptr<ThemeGameMode> m_currentTheme;
  std::string m_currentThemeName;

  double m_resize = 1.0f;

};
}

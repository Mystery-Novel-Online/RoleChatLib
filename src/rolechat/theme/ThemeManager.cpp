#include "rolechat/theme/ThemeManager.h"

bool rolechat::theme::ThemeManager::LoadTheme(const std::string &name)
{
  auto theme = std::make_unique<ThemeGameMode>("themes/" + name);

  if (!theme->valid())
    return false;

  m_currentTheme = std::move(theme);
  m_currentThemeName = name;

  return true;
}

void rolechat::theme::ThemeManager::ReloadTheme()
{
  LoadTheme(m_currentThemeName);
}

void rolechat::theme::ThemeManager::SwitchGamemode(const std::string &gamemode)
{
  if(!m_currentTheme->valid())
    return;
  m_currentTheme->setCurrentGamemode(gamemode);
}

void rolechat::theme::ThemeManager::SwitchTime(const std::string &time)
{
  if(!m_currentTheme->valid())
    return;
  m_currentTheme->setCurrentTimeMode(time);
}

const rolechat::theme::ThemeElement &rolechat::theme::ThemeManager::GetElement(ThemeSceneType scene, const std::string &id)
{
  static ThemeElement fallback = {};

  const ThemeElement *element = m_currentTheme->getElement(scene, id);
  if(!element)
    return fallback;

  return *element;
}

#ifndef ROLECHAT_THEME_THEME_GAME_MODE_H
#define ROLECHAT_THEME_THEME_GAME_MODE_H   
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

#include "rolechat/filesystem/RCDir.h"
#include "rolechat/theme/ThemeModule.h"

namespace rolechat::theme {
class ThemeGameMode {
public:
    ThemeGameMode(std::string gamemodePath);
    virtual ~ThemeGameMode() = default;

    void setCurrentGamemode(const std::string& gamemode) { m_currentGamemode = gamemode; }
    void setCurrentTimeMode(const std::string& timeMode);

    const ThemeElement* getElement(ThemeSceneType scene, const std::string& identifier) const;

    std::vector<std::string> moduleNames() const;
    std::vector<std::string> gamemodes() const;
    std::vector<std::string> timemodes() const;

private:
    std::string m_currentGamemode;
    std::string m_currentTimeMode;

    std::unordered_map<std::string, ThemeModule> m_modeModules;
    std::unordered_map<std::string, std::unique_ptr<ThemeGameMode>> m_gameModes;
    std::unordered_map<std::string, std::unique_ptr<ThemeGameMode>> m_timeModes;
    fs::RCDir m_gamemodeDirectory;
};
}
#endif
#ifndef ROLECHAT_THEME_THEME_GAME_MODE_H
#define ROLECHAT_THEME_THEME_GAME_MODE_H   
#include <string>
#include "rolechat/filesystem/RCDir.h"
#include "rolechat/theme/ThemeModule.h"

namespace rolechat::theme {
class ThemeGameMode {
public:
    ThemeGameMode(std::string gamemodePath);
    virtual ~ThemeGameMode() = default;

    std::vector<std::string> moduleNames() const;
    std::vector<std::string> gamemodes() const;
    std::vector<std::string> timemodes() const;

private:
    std::unordered_map<std::string, ThemeModule> m_modeModules;
    std::unordered_map<std::string, ThemeGameMode> m_gameModes;
    std::unordered_map<std::string, ThemeGameMode> m_timeModes;
    fs::RCDir m_gamemodeDirectory;
};
}
#endif
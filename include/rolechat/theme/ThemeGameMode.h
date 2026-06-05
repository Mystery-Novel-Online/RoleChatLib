#ifndef ROLECHAT_THEME_THEME_GAME_MODE_H
#define ROLECHAT_THEME_THEME_GAME_MODE_H
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <variant>

#include "rolechat/filesystem/RCDir.h"
#include "rolechat/theme/ThemeModule.h"

using ThemeConfigValue = std::variant<bool, int, double, std::string>;

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

    bool valid() {  return m_validMode; };

private:
    std::optional<ThemeModule> m_baseModule = std::nullopt;
    std::string m_currentGamemode;
    std::string m_currentTimeMode;

    std::unordered_map<std::string, ThemeModule> m_modeModules;
    std::unordered_map<std::string, std::unique_ptr<ThemeGameMode>> m_gameModes;
    std::unordered_map<std::string, std::unique_ptr<ThemeGameMode>> m_timeModes;
    fs::RCDir m_gamemodeDirectory;

    bool m_validMode = false;
};
}
#endif

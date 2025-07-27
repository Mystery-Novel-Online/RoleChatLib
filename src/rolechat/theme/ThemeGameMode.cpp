#include "rolechat/theme/ThemeGameMode.h"
#include <algorithm>

using namespace rolechat::theme;

ThemeGameMode::ThemeGameMode(std::string gamemodePath) : m_gamemodeDirectory(gamemodePath), m_currentGamemode("default"), m_currentTimeMode("default")
{
    using namespace rolechat::fs;
    if (!m_gamemodeDirectory.exists()) 
        return;

    RCDir moduleDir(gamemodePath + "/modules");
    if (moduleDir.exists()){
        for (const auto& moduleName : moduleDir.subDirectories()) 
            m_modeModules.emplace(moduleName, ThemeModule(gamemodePath, moduleName));
    } 

    RCDir gamemodeDir(gamemodePath + "/gamemodes");
    if (gamemodeDir.exists()){
        for (const auto& gamemodeName : gamemodeDir.subDirectories()) 
            m_gameModes.emplace(gamemodeName, std::make_unique<ThemeGameMode>(gamemodePath + "/gamemodes/" + gamemodeName));
    } 

    RCDir timesDir(gamemodePath + "/times");
    if (timesDir.exists()) {
        for (const auto& timeModeName : timesDir.subDirectories()) 
            m_timeModes.emplace(timeModeName, std::make_unique<ThemeGameMode>(gamemodePath + "/times/" + timeModeName));
    } 
}

std::vector<std::string> ThemeGameMode::moduleNames() const
{
    std::vector<std::string> names;
    for (const auto& pair : m_modeModules) {
        names.push_back(pair.first);
    }
    std::sort(names.begin(), names.end());
    return names;
}

std::vector<std::string> ThemeGameMode::gamemodes() const
{
    std::vector<std::string> names;
    for (const auto& pair : m_gameModes) {
        names.push_back(pair.first);
    }
    std::sort(names.begin(), names.end());
    return names;
}

std::vector<std::string> ThemeGameMode::timemodes() const
{
    std::vector<std::string> names;
    for (const auto& pair : m_timeModes) {
        names.push_back(pair.first);
    }
    std::sort(names.begin(), names.end());
    return names;
}

void ThemeGameMode::setCurrentTimeMode(const std::string &timeMode)
{
    m_currentTimeMode = timeMode;
    for (auto& pair : m_gameModes) {
        pair.second->setCurrentTimeMode(timeMode);
    }
}

const ThemeElement *ThemeGameMode::getElement(ThemeSceneType scene, const std::string &identifier) const
{
    auto it = m_gameModes.find(m_currentGamemode);
    if (it != m_gameModes.end()) {
        const ThemeElement* element = it->second->getElement(scene, identifier);
        if (element) return element;
    }

    auto timeIt = m_timeModes.find(m_currentTimeMode);
    if (timeIt != m_timeModes.end()) {
        const ThemeElement *element = timeIt->second->getElement(scene, identifier);
        if(element)
            return element;
    }

    for (auto& pair : m_modeModules) {
        const ThemeElement *element = pair.second.getElement(scene, identifier);
        if(element)
            return element;
    }

    return nullptr;
}

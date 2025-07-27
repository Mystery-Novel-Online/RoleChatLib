#include "rolechat/theme/ThemeGameMode.h"
#include <algorithm>

rolechat::theme::ThemeGameMode::ThemeGameMode(std::string gamemodePath) : m_gamemodeDirectory(gamemodePath)
{
    using namespace rolechat::fs;
    if (!m_gamemodeDirectory.exists()) 
        return;

    RCDir moduleDir(gamemodePath + "/modules");
    if (moduleDir.exists())
    {
        auto modules = moduleDir.subDirectories();
        for (const auto& moduleName : modules) 
        {
            m_modeModules.emplace(moduleName, ThemeModule(gamemodePath, moduleName));
        }
    } 

    
    RCDir gamemodeDir(gamemodePath + "/gamemodes");
    if (gamemodeDir.exists())
    {
        auto gamemodesFound = gamemodeDir.subDirectories();
        for (const auto& gamemodeName : gamemodesFound) 
        {
            m_gameModes.emplace(gamemodeName, ThemeGameMode(gamemodePath + "/gamemodes/" + gamemodeName));
        }
    } 

    
    RCDir timesDir(gamemodePath + "/times");
    if (timesDir.exists())
    {
        auto gamemodesFound = timesDir.subDirectories();
        for (const auto& gamemodeName : gamemodesFound) 
        {
            m_timeModes.emplace(gamemodeName, ThemeGameMode(gamemodePath + "/times/" + gamemodeName));
        }
    } 
}

std::vector<std::string> rolechat::theme::ThemeGameMode::moduleNames() const
{
    std::vector<std::string> names;
    for (const auto& pair : m_modeModules) {
        names.push_back(pair.first);
    }
    std::sort(names.begin(), names.end());
    return names;
}

std::vector<std::string> rolechat::theme::ThemeGameMode::gamemodes() const
{
    std::vector<std::string> names;
    for (const auto& pair : m_gameModes) {
        names.push_back(pair.first);
    }
    std::sort(names.begin(), names.end());
    return names;
}

std::vector<std::string> rolechat::theme::ThemeGameMode::timemodes() const
{
    std::vector<std::string> names;
    for (const auto& pair : m_timeModes) {
        names.push_back(pair.first);
    }
    std::sort(names.begin(), names.end());
    return names;
}

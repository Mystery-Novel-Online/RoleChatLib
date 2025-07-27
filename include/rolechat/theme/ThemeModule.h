#ifndef ROLECHAT_THEME_THEME_MODULE_H
#define ROLECHAT_THEME_THEME_MODULE_H
#include <string>
#include <unordered_map>
#include <vector>
#include "rolechat/filesystem/RCDir.h"
#include "rolechat/theme/ThemeScene.h"
#include "rolechat/theme/ThemeHighlight.h"

namespace rolechat::theme {
class ThemeModule {
public:
    ThemeModule(std::string gamemodePath, std::string moduleName);
    virtual ~ThemeModule() = default;

    const ThemeElement* getElement(ThemeSceneType scene, const std::string& identifier) const;

private:
    std::string m_moduleName;
    std::string m_gamemodePath;

    std::unordered_map<ThemeSceneType, ThemeScene> m_scenes;
    std::unordered_map<std::string, ThemeHighlight> m_highlights;

    fs::RCDir m_moduleDir;
};
}
#endif
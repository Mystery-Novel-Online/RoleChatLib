#include "rolechat/theme/ThemeModule.h"
#include "rolechat/filesystem/RCDir.h"
#include "rolechat/filesystem/RCFile.h"
#include <iostream>
#include "rolechat/util/JsonUtils.h"
#include "rolechat/theme/ThemeElement.h"

using namespace rolechat::theme;
ThemeModule::ThemeModule(std::string gamemodePath, std::string moduleName) : m_gamemodePath(gamemodePath), m_moduleName(moduleName), m_moduleDir(gamemodePath + "/modules/" + moduleName) 
{
    if (!m_moduleDir.exists()) 
        return;

    m_scenes[ThemeSceneType::SceneType_ServerSelect] = ThemeScene();
    m_scenes[ThemeSceneType::SceneType_Courtroom] = ThemeScene();
    m_scenes[ThemeSceneType::SceneType_Viewport] = ThemeScene();
    m_scenes[ThemeSceneType::SceneType_Replays] = ThemeScene();

    std::string moduleConfigPath = m_moduleDir.findFirst() + "/default.json";
    if(fs::RCFile::exists(moduleConfigPath)) 
    {
        std::cout << "Loaded theme module configuration from: " << moduleConfigPath << std::endl;
    } 
    else 
    {
        std::cerr << "Theme module configuration file not found: " << moduleConfigPath << std::endl;
    }

    JsonUtils::JsonData json = JsonUtils::loadFile(moduleConfigPath);

    for (const auto& [identifier, elementObject] : json["courtroom"].items()) 
    {
        if (!elementObject.is_object()) continue;

        ThemeElement element;
        element.position.x = elementObject["position"]["x"].get<int>();
        element.position.y = elementObject["position"]["y"].get<int>();
        element.position.width = elementObject["position"]["width"].get<int>();
        element.position.height = elementObject["position"]["height"].get<int>();

        element.font.name = elementObject["font"]["name"].get<std::string>();
        element.font.color = elementObject["font"]["color"].get<std::string>();
        element.font.alignment = elementObject["font"]["alignment"].get<std::string>();
        element.font.size = elementObject["font"]["size"].get<int>();
        element.font.bold = elementObject["font"].value("bold", false);
        element.font.italic = elementObject["font"].value("italic", false);
        element.font.outline = elementObject["font"].value("outline", false);

        m_scenes[ThemeSceneType::SceneType_Courtroom].elements[identifier] = element;
    }
}

const ThemeElement *ThemeModule::getElement(ThemeSceneType scene, const std::string &identifier) const
{
    auto sceneIt = m_scenes.find(scene);
    if (sceneIt == m_scenes.end())
        return nullptr;
    const auto& elements = sceneIt->second.elements;
    auto elemIt = elements.find(identifier);
    if (elemIt == elements.end())
        return nullptr;
    return &elemIt->second;
}

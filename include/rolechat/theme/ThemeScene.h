#ifndef ROLECHAT_THEME_THEME_SCENE_H
#define ROLECHAT_THEME_THEME_SCENE_H

#include <string>
#include <unordered_map>
#include "rolechat/theme/ThemeElement.h"

namespace rolechat::theme {

enum class ThemeSceneType {
    SceneType_INVALID = -1,
    SceneType_ServerSelect,
    SceneType_Courtroom,
    SceneType_Viewport,
    SceneType_Replays
};

struct ThemeScene {
    std::unordered_map<std::string, ThemeElement> elements;
};
}

#endif
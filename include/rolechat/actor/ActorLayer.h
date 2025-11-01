#pragma once
#include <string>
#include "rolechat/math/RPRect.h"

struct ActorLayer
{
    std::string offsetName;
    std::string spriteName;
    std::string spriteOrder;

    RPRect layerOffset;

    bool detachLayer;

    std::string blendMode;
    std::string toggleName;
    std::string assetPath;

    bool defaultDisabled;
    std::vector<std::string> variationOptions = {};
};
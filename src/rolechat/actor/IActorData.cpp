#include "rolechat/actor/IActorData.h"

using namespace rolechat::actor;

ActorScalingMode IActorData::scalingMode() const
{
    static const std::unordered_map<std::string, ActorScalingMode> scalingModeMap = 
    {
        {"width_smooth", ActorScalingMode::WidthSmoothScaling},
        {"width_pixels", ActorScalingMode::WidthPixelScaling},
        {"automatic", ActorScalingMode::AutomaticScaling}
    };

    std::string mode = m_scalingMode;
    std::transform(mode.begin(), mode.end(), mode.begin(), ::tolower);

    auto it = scalingModeMap.find(mode);
    if (it != scalingModeMap.end()) return it->second;
    return ActorScalingMode::AutomaticScaling;
}

std::string IActorData::buttonImage(const ActorEmote &emote, bool enabled) const
{
    std::string state = enabled ? "on" : "off";
    return "emotions/button" + emote.key + "_" + state + ".png";
}

std::string IActorData::selectedImage(const ActorEmote &emote) const
{
    return "emotions/selected.png";
}


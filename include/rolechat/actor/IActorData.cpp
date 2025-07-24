#include "rolechat/actor/IActorData.h"
#include "IActorData.h"

using namespace rolechat::actor;

ActorScalingMode IActorData::scalingMode() const
{
    static const std::unordered_map<std::string, ActorScalingMode> scalingModeMap = 
    {
        {"width_smooth", ActorScalingMode::WidthSmoothScaling},
        {"width_pixels", ActorScalingMode::WidthPixelScaling},
        {"automatic", ActorScalingMode::Automatic}
    };

    std::string mode = m_scalingMode;
    std::transform(mode.begin(), mode.end(), mode.begin(), ::tolower);

    auto it = scalingModeMap.find(mode);
    if (it != scalingModeMap.end()) return it->second;
    return ActorScalingMode::Automatic;
}

std::string rolechat::actor::IActorData::buttonImage(const ActorEmote &emote, bool enabled)
{
    std::string state = enabled ? "on" : "off";
    return "emotions/button" + emote.key + "_" + state + ".png";
}

std::string rolechat::actor::IActorData::selectedImage(const ActorEmote &emote)
{
    return "emotions/selected.png";
}


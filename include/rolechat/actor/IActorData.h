#ifndef ROLECHAT_ACTOR_IACTORDATA_H
#define ROLECHAT_ACTOR_IACTORDATA_H

#pragma once

#include <string>
#include "rolechat/actor/ActorEmote.h"
#include "rolechat/actor/ActorLayer.h"
#include "rolechat/actor/ActorOutfit.h"
#include "rolechat/actor/ActorScalingPreset.h"
#include "rolechat/actor/ActorScalingMode.h"

namespace rolechat::actor {

/**
 * @brief Interface for actor data management.
 */
class IActorData {
public:
    IActorData() = default;
    virtual ~IActorData() = default;

    virtual void load(const std::string& folder) = 0;
    virtual void reload() {}

    virtual std::string outfit()    const { return m_currentOutfit; }
    virtual std::string folder()    const { return m_folder; }
    virtual std::string showname()  const { return m_showname.empty() ? m_folder : m_showname;}
    virtual std::string gender()    const { return m_gender; }
    virtual std::string side()      const { return m_side; }
    virtual ActorScalingMode scalingMode() const;

    virtual std::vector<ActorScalingPreset> scalingPresets() const { return m_presets; }
    virtual std::vector<std::string> outfitNames() const {  return {}; }
    virtual std::vector<ActorLayer>& emotes() const { return {}; }

    virtual std::string buttonImage(const ActorEmote& emote, bool enabled);
    virtual std::string selectedImage(const ActorEmote& emote);

    virtual void switchOutfit(const std::string& outfit) { m_currentOutfit = outfit; }
    virtual void setFolder(const std::string& folder) { m_folder = folder; }
    virtual void setShowname(const std::string& showname) { m_showname = showname;}
    virtual void setGender(const std::string& gender) { m_gender = gender;}
    virtual void setSide(const std::string& side) {m_side = side;}
    virtual void setScalingMode(const std::string& mode) {m_scalingMode = mode;}
    virtual void setScalingPresets(std::vector<ActorScalingPreset> presets) { m_presets = presets; }

private:
    std::vector<ActorScalingPreset> m_presets = {};
    std::string m_folder = "";
    std::string m_showname = "";
    std::string m_gender = "female";
    std::string m_side = "wit";
    std::string m_currentOutfit = "default";
    std::string m_scalingMode = "automatic";
};

}

#endif // ROLECHAT_ACTOR_IACTORDATA_H

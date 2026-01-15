#ifndef ROLECHAT_ACTOR_JSONACTORDATA_H
#define ROLECHAT_ACTOR_JSONACTORDATA_H

#pragma once

#include "rolechat/actor/IActorData.h"

namespace rolechat::actor {

/**
 * @brief Interface for actor data management.
 */
class JsonActorData : public IActorData {
public:
    JsonActorData() = default;

    void load(const std::string& folder, const std::string& path) override;
    void reload() override;

    std::unordered_map<std::string, actor::ActorOutfit*> outfits() const override {return m_outfits; }
    std::string showname() const override;
    std::vector<std::string> outfitNames() const override { return m_outfitNames; }
    std::vector<ActorEmote> emotes() override;

    std::string buttonImage(const ActorEmote& emote, bool enabled) const override;
    std::string selectedImage(const ActorEmote& emote) const override;

    void switchOutfit(const std::string& outfit) override;

private:
    bool m_validCharacter = false;
    std::unordered_map<std::string, actor::ActorOutfit*> m_outfits = {};
    std::vector<std::string> m_outfitNames = {};
    std::vector<std::string> m_outfitsOrder = {};
    std::unordered_map<std::string, std::time_t> m_outfitModifiedTimes = {};
};

}

#endif // ROLECHAT_ACTOR_IACTORDATA_H

#ifndef ROLECHAT_ACTOR_ACTOR_USER_MANAGER_H
#define ROLECHAT_ACTOR_ACTOR_USER_MANAGER_H

#pragma once

#include <unordered_map>
#include <string>

#include "rolechat/actor/IActorData.h"

namespace rolechat::actor {
/**
 * @brief Interface for managing data for the users current actor.
 */
class ActorUserManager {
public:
    ActorUserManager() = default;
    virtual ~ActorUserManager() = default;

    IActorData *userActor() const { return m_userActor; }
    std::string userActorName() const { return m_userActorName; }
    
    bool layerState(const std::string& layer) const {
        auto it = m_actorLayerStates.find(layer);
        return it != m_actorLayerStates.end() ? it->second : true;
    }

    void setLayerState(const std::string& layer, bool state) { m_actorLayerStates[layer] = state; }


private:
    IActorData *m_userActor = nullptr;
    std::string m_userActorName = "";
    std::unordered_map<std::string, bool> m_actorLayerStates = {};
};

}

#endif // ROLECHAT_ACTOR_IACTORDATA_H

#pragma once
#include <string>
#include "rolechat/actor/ActorEmote.h"
#include "rolechat/util/JsonUtils.h"

namespace rolechat::actor {
using namespace rolechat::JsonUtils;

class ActorOutfit {
public:
    ActorOutfit(const std::string& character, const std::string& outfit, const std::string& characterPath);

    void setShowname(const std::string& showname) { m_showname = showname;};
    std::string showname() { return m_showname;};

    const std::vector<ActorEmote>& emotes() const noexcept { return m_emotes; }
    const std::vector<ActorLayer>& layers() const noexcept { return m_layers; }

private:
    void readEmotes(const JsonData& data);

    std::string m_name = "";
    std::string m_character = "";
    std::string m_showname = "";
    std::string m_path = "";

    bool m_showDesk = true;
    bool m_ignoreOffsets = false;

    std::vector<ActorEmote> m_emotes = {};
    std::vector<ActorLayer> m_layers = {};
};

} 

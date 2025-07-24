#pragma once
#include <string>
#include <vector>
#include "rolechat/actor/ActorLayer.h"

struct ActorEmote
{
    std::string character;
    std::string key;
    std::string comment;
    std::string anim;
    std::string sequence;
    std::string dialog;
    int modifier = 0;
    int desk_modifier = -1;
    bool ignore_offsets = false;
    std::string sound_file;
    int sound_delay = 0;
    std::string video_file;
    std::string outfitName = "";
    std::string emoteName = "";
    std::vector<ActorLayer> emoteOverlays = {};
};
#ifndef ROLECHAT_DIALOGUE_DIALOGUE_CHARACTER_H
#define ROLECHAT_DIALOGUE_DIALOGUE_CHARACTER_H
#include <string>
namespace rolechat::dialogue {
struct DialogueCharacter {
    std::string character;
    std::string outfit;
    std::string emote;
    std::string layerData;
    std::string keyframeSequence;

    int offsetScale = 1000;
    int offsetHorizontal = 500;
    int offsetVertical = 0;

    bool isFlipped = false;
    bool isVisible = false;

    bool isLeader = false;
};
}
#endif
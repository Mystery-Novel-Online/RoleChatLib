#ifndef ROLECHAT_DIALOGUE_DIALOGUE_MESSAGE_H
#define ROLECHAT_DIALOGUE_DIALOGUE_MESSAGE_H
#include <string>
#include "rolechat/dialogue/DialogueCharacter.h"
#include "rolechat/dialogue/DialogueColor.h"
#include "rolechat/dialogue/DialogueTypes.h"
namespace rolechat::dialogue {
struct DialogueMessage {
    int speakerClient = 0;
    int speakerCharacter = 0;
    DialogueCharacter speaker;
    DialogueCharacter partner;

    DialogueType type = DialogueTalk; 
    DialogueColor color = CDefault;

    std::string showname;
    std::string sfx;
    std::string video;
    std::string preAnimation;
    std::string tags;
    std::string message;
    std::string areaPosition;
    
    bool playAnimation = false;

    //MessageEffect effect;
    int effectState;
    int delatSfx;
    int shoutModifier;

    bool deskVisible = true;
};
}
#endif
#ifndef ROLECHAT_DIALOGUE_DIALOGUE_EMOTE_MODE_H
#define ROLECHAT_DIALOGUE_DIALOGUE_EMOTE_MODE_H
#include <string>
namespace rolechat::dialogue {
enum DialogueEmoteMode : int32_t
{
  Idle = 0,
  PreAnimation = 1
};
}
#endif
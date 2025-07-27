#ifndef ROLECHAT_DIALOGUE_DIALOGUE_COLOR_H
#define ROLECHAT_DIALOGUE_DIALOGUE_COLOR_H
#include <string>
namespace rolechat::dialogue {
enum DialogueColor : int32_t
{
  CDefault,
  CGreen,
  CRed,
  COrange,
  CBlue,
  CYellow,
  CPurple,
  CPink,
  CRainbow,
  CWhite = CDefault,
};
}
#endif
#ifndef ROLECHAT_DIALOGUE_DIALOGUE_TYPE_H
#define ROLECHAT_DIALOGUE_DIALOGUE_TYPE_H
#include <string>
namespace rolechat::dialogue {
enum DialogueType
{
  DialogueTalk,
  DialogueShout,
  DialogueThink,
  DialogueCG,
  DialogueNarrator
};
}
#endif
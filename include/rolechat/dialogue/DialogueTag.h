#ifndef ROLECHAT_DIALOGUE_DIALOGUE_TAG_H
#define ROLECHAT_DIALOGUE_DIALOGUE_TAG_H
#include <string>
namespace rolechat::dialogue {
struct DialogueTag
{
  int id;
  int index;
  std::string image;
  std::string value;
};
}
#endif
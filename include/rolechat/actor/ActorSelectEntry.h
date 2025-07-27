#ifndef ROLECHAT_ACTOR_ACTOR_SELECT_ENTRY_H
#define ROLECHAT_ACTOR_ACTOR_SELECT_ENTRY_H
#include <string>
namespace rolechat::actor {
struct ActorSelectEntry {
    std::string name;
    bool taken = false;
};
}
#endif
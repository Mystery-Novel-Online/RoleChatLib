#ifndef ROLECHAT_THEME_THEME_GROUP_H
#define ROLECHAT_THEME_THEME_GROUP_H
#include <string>
#include <vector>
#include "rolechat/math/RPRect.h"
namespace rolechat::theme {
struct ThemeGroup {
    std::string name;
    std::string group;
    bool dragEnabled = false;
    bool toggleEnabled = false;
    RPRect transform;
    std::vector<std::string> elementsList;
};
}
#endif
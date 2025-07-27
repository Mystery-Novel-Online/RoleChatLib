#ifndef ROLECHAT_THEME_THEME_HIGHLIGHT_H
#define ROLECHAT_THEME_THEME_HIGHLIGHT_H
#include <string>
namespace rolechat::theme {
struct ThemeHighlight
{
  std::string chars = "";
  std::string color = "#FFFFFF";
  bool keepCharacters = false;
};

}
#endif
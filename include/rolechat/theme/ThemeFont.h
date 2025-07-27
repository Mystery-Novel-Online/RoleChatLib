#ifndef ROLECHAT_THEME_THEME_FONT_H
#define ROLECHAT_THEME_THEME_FONT_H
#include <string>
namespace rolechat::theme {
struct ThemeFont {
    std::string name;
    std::string color;
    std::string alignment; 
    int size;
    bool bold = false;
    bool italic = false;
    bool outline = false;
    std::string outlineColor;
    int outlineSize = 1;
};
}
#endif
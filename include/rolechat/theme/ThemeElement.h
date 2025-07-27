#ifndef ROLECHAT_THEME_THEME_ELEMENT_H
#define ROLECHAT_THEME_THEME_ELEMENT_H

#include <string>
#include "rolechat/math/RPRect.h"
#include "rolechat/theme/ThemeFont.h"
namespace rolechat::theme {
struct ThemeElement {
    RPRect position;
    ThemeFont font;
};
}
#endif
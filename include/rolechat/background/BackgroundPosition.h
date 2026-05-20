#pragma once
#include <map>
#include <string>
namespace rolechat::background {
struct BackgroundPosition {
    std::string background;
    std::string foreground;
    std::string ambientSfx;

    std::map<std::string, BackgroundPosition> time_variants;
};
}

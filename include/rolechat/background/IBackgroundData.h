#ifndef ROLECHAT_AREA_BACKGROUND_H
#define ROLECHAT_AREA_BACKGROUND_H

#include <string>
#include <optional>
#include <unordered_map>
#include "BackgroundPosition.h"

namespace rolechat::background {

class IBackgroundData {
public:
    IBackgroundData() = default;

    void assignPosition(const std::string& position, BackgroundPosition data);
    std::string backgroundFilename(const std::string& position);
    std::string foregroundFilename(const std::string& position);

    virtual void loadBackground(const std::string& backgroundPath) = 0;
private:
    std::unordered_map<std::string, BackgroundPosition> m_backgroundPositions;
};

} 

#endif // ROLECHAT_THEME_H

#include "rolechat/background/IBackgroundData.h"

using namespace rolechat::background;

void IBackgroundData::assignPosition(const std::string & position, BackgroundPosition data)
{
    m_backgroundPositions[position] = data;
}

std::string IBackgroundData::backgroundFilename(const std::string &position)
{
    if (m_backgroundPositions.find(position) != m_backgroundPositions.end()) {
        return m_backgroundPositions[position].background;
    } else {
        return "";
    }
}
std::string IBackgroundData::foregroundFilename(const std::string &position)
{
    if (m_backgroundPositions.find(position) != m_backgroundPositions.end()) {
        return m_backgroundPositions[position].foreground;
    } else {
        return "";
    }
}
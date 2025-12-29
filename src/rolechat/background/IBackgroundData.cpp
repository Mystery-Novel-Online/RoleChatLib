#include "rolechat/background/IBackgroundData.h"

using namespace rolechat::background;

void IBackgroundData::assignPosition(const std::string& variant, const std::string & position, BackgroundPosition data)
{
  if(m_currentVariant.empty())
  {
    m_defaultVariant = variant;
    m_currentVariant = variant;
  }
  m_backgroundPositions[variant][position] = data;
}

void IBackgroundData::setVariant(const std::string &variant)
{
  if (m_backgroundPositions.find(variant) != m_backgroundPositions.end()) {
    m_currentVariant = variant;
  }
  else
  {
    m_currentVariant = m_defaultVariant;
  }
}

std::string IBackgroundData::backgroundFilename(const std::string &position)
{
    if (m_backgroundPositions.find(m_currentVariant) != m_backgroundPositions.end()) {
      if (m_backgroundPositions[m_currentVariant].find(position) != m_backgroundPositions[m_currentVariant].end()) {
        return m_backgroundPositions[m_currentVariant][position].background;
      }
    } else {
        return "";
    }
}
std::string IBackgroundData::foregroundFilename(const std::string &position)
{
  if (m_backgroundPositions.find(m_currentVariant) != m_backgroundPositions.end()) {
    if (m_backgroundPositions[m_currentVariant].find(position) != m_backgroundPositions[m_currentVariant].end()) {
      return m_backgroundPositions[m_currentVariant][position].foreground;
    }
  } else {
        return "";
    }
}

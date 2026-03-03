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
  const auto variantIt = m_backgroundPositions.find(m_currentVariant);
  if (variantIt == m_backgroundPositions.end())
    return {};

  const auto& positions = variantIt->second;

  const auto posIt = positions.find(position);
  if (posIt == positions.end())
  {
    const auto defaultPosIt = positions.find("Default");
    if(defaultPosIt != positions.end())
    {
      return defaultPosIt->second.background;
    }
    return {};
  }

  return posIt->second.background;

}
std::string IBackgroundData::foregroundFilename(const std::string &position)
{
  const auto variantIt = m_backgroundPositions.find(m_currentVariant);
  if (variantIt == m_backgroundPositions.end())
    return {};

  const auto& positions = variantIt->second;
  const auto posIt = positions.find(position);
  if (posIt == positions.end())
  {
    const auto defaultPosIt = positions.find("Default");
    if(defaultPosIt != positions.end())
    {
      return defaultPosIt->second.foreground;
    }
    return {};
  }

  return posIt->second.foreground;
}

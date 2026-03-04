#include "rolechat/viewport_scene.h"
#include <rolechat/filesystem/RCDir.h>
#include <rolechat/filesystem/RCFile.h>
#include <rolechat/background/JsonBackgroundData.h>

using ApplicationDirectory = rolechat::fs::RCDir;
using ApplicationFile = rolechat::fs::RCFile;

bool ViewportScene::switchBackground(const std::string &name, const std::string &variant)
{
  m_sceneBackground.reset();
  m_backgroundName.emplace(name);

  ApplicationDirectory bgDirectory("background/" + name);
  if(!bgDirectory.exists())
    return false;

  ApplicationFile bgFile("background/" + name + "/background.json");
  if(!bgFile.exists())
    return false;

  m_sceneBackground.emplace(std::make_unique<background::JsonBackgroundData>());
  m_sceneBackground.value()->loadBackground(bgFile.findFirst());
  m_sceneBackground.value()->setVariant(variant);
  return true;
}

std::string ViewportScene::backgroundFile(const std::string &position)
{
  if(!m_backgroundName.has_value())
    return "";

  if(!m_sceneBackground.has_value())
    return "";

  const std::string& name = m_backgroundName.value();

  std::string fileName = m_sceneBackground.value()->backgroundFilename(position);
  ApplicationFile bgFile("background/" + name + "/" + fileName);

  std::string firstFile = bgFile.findFirst();
  return firstFile;
}

std::string ViewportScene::foregroundFile(const std::string &position)
{
  if(!m_backgroundName.has_value())
    return "";

  if(!m_sceneBackground.has_value())
    return "";

  const std::string& name = m_backgroundName.value();

  ApplicationFile bgFile("background/" + name + "/" + m_sceneBackground.value()->foregroundFilename(position));
  return bgFile.findFirst();
}

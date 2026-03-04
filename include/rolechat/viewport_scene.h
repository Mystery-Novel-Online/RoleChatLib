#ifndef VIEWPORT_SCENE_H
#define VIEWPORT_SCENE_H
#include <memory>
#include <optional>
#include "rolechat/background/IBackgroundData.h"

using namespace rolechat;
class ViewportScene
{
public:
  virtual bool switchBackground(const std::string& name, const std::string& variant);

  std::string backgroundFile(const std::string& position);
  std::string foregroundFile(const std::string& position);
  std::vector<std::string> positions()
  {
    if(!m_sceneBackground.has_value()) return {};
    return m_sceneBackground.value()->positionsList();
  }

  std::optional<std::unique_ptr<background::IBackgroundData>>& background() { return m_sceneBackground; }

private:
  std::optional<std::string> m_backgroundName;
  std::optional<std::unique_ptr<background::IBackgroundData>> m_sceneBackground;

};

#endif // VIEWPORT_SCENE_H

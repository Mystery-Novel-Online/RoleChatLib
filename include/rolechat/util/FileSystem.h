#pragma once
#include <string>
#include <vector>

namespace rolechat::fs
{
class PackageManager {
public:
  static std::vector<std::string> scanPackages();
  
  static std::vector<std::string> packageNames();
  static std::vector<std::string> disabledList();

  static void setDisabledList(std::vector<std::string> disableList);
  static void saveDisabled();

private:
  static std::vector<std::string> m_packageNames;
  static std::vector<std::string> m_disabledPackages;
};

}

namespace rolechat::fs::checks
{
  bool directoryExists(const std::string& path);
  bool characterExists(const std::string& character);
}

namespace rolechat::fs::paths
{
  std::string applicationPath();
  std::string basePath();
  std::string packagePath(const std::string& packageName);
}

namespace rolechat::fs::formats
{
  std::vector<std::string> supportedAudio(bool allowExtensionless = false);

  std::vector<std::string> supportedImages();
  std::vector<std::string> animatedImages();
  std::vector<std::string> staticImages(); 
}

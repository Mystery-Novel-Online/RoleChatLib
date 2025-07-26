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
  bool fileExists(const std::string& path);
  bool directoryExists(const std::string& path);
  bool characterExists(const std::string& character);
}

namespace rolechat::fs::paths
{
  std::string applicationPath();
  std::string basePath();
  std::string packagePath(const std::string& packageName);

  std::vector<std::string> findFiles(const std::string& filePath, const std::vector<std::string> &extensions = {});

  std::string findDirectory(const std::string& directoryPath, bool allowPackages = true, bool absolutePath = true);
  std::vector<std::string> getDirectoryList(const std::string& directoryPath, bool includePackages = true);
  std::vector<std::string> getFileList(const std::string& directoryPath, bool includePackages = true, const std::string& extensionFilter = "", bool includeExtension = false);
  std::vector<std::string> getFileList(const std::string& directoryPath, const std::string& packageName, const std::string& extensionFilter = "", bool includeExtension = false);

}

namespace rolechat::fs::formats
{
  std::vector<std::string> supportedAudio(bool allowExtensionless = false);

  std::vector<std::string> supportedImages();
  std::vector<std::string> animatedImages();
  std::vector<std::string> staticImages(); 
}

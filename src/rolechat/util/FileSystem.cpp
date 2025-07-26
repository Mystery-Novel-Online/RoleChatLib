#include "rolechat/util/FileSystem.h"
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "rolechat/filesystem/RCDir.h"
using namespace rolechat::fs;

bool checks::directoryExists(const std::string &path)
{
    return std::filesystem::exists(path) && std::filesystem::is_directory(path);
}

bool checks::characterExists(const std::string &path)
{
    return RCDir("characters/" + path).exists();
}

// Implementation of PackageManager variables

namespace rolechat::fs {
    std::vector<std::string> PackageManager::m_packageNames;
    std::vector<std::string> PackageManager::m_disabledPackages;
}

// Implementation of PackageManager methods
std::vector<std::string> PackageManager::scanPackages()
{
    m_packageNames.clear();

    // Scan the packages directory for subdirectories
    std::string packagesPath = paths::applicationPath() + "/packages/";
    if(checks::directoryExists(packagesPath)) 
    {
        for (const auto& entry : std::filesystem::directory_iterator(packagesPath)) 
        {
           if (entry.is_directory()) 
           {
               std::string packageName = entry.path().filename().string();
              if (!packageName.empty()) 
                    m_packageNames.push_back(packageName);
            }
        }
    } 


    // Check for disabled packages configuration
    std::string iniPath = paths::basePath() + "packages.ini";
    std::ifstream iniFile(iniPath);
    m_disabledPackages.clear();
    if (iniFile.is_open()) 
    {
        std::string line;
        while (std::getline(iniFile, line)) 
        {
            line.erase(line.find_last_not_of(" \r\n") + 1);
            if (std::find(m_packageNames.begin(), m_packageNames.end(), line) != m_packageNames.end()) 
                m_disabledPackages.push_back(line);
        }
    }

    return m_packageNames;
}

std::vector<std::string> PackageManager::packageNames()
{
    return m_packageNames;
}

std::vector<std::string> PackageManager::disabledList()
{
    return m_disabledPackages;
}

void PackageManager::setDisabledList(std::vector<std::string> disableList)
{
    m_disabledPackages.clear();
    m_disabledPackages = disableList;
    saveDisabled();
}

void PackageManager::saveDisabled()
{
    std::string iniPath = paths::basePath() + "packages.ini";
    std::ofstream iniFile(iniPath, std::ios::trunc);

    if (!iniFile.is_open())
        return;

    for (const auto& pkg : m_disabledPackages)
    {
        iniFile << pkg << "\r\n";
    }
}

// Implementation of paths functions
std::string paths::applicationPath()
{
    auto path = std::filesystem::current_path();
#if defined(__APPLE__)
    for (int i = 0; i < 3; ++i)
        path = path.parent_path();
#endif
    return path.string();
}

std::string paths::basePath()
{
    return applicationPath() + "/base/";
}

std::string paths::packagePath(const std::string &packageName)
{
    return applicationPath() + "/packages/" + packageName + "/";
}


std::vector<std::string> formats::supportedAudio(bool allowExtensionless)
{
    static const std::vector<std::string> s_ext_list{".wav", ".ogg", ".opus", ".mp3"};
    if (!allowExtensionless)
        return s_ext_list;

    std::vector<std::string> withEmpty = {""};
    withEmpty.insert(withEmpty.end(), s_ext_list.begin(), s_ext_list.end());
    return withEmpty;
}

std::vector<std::string> formats::supportedImages()
{
    std::vector<std::string> result = animatedImages();
    std::vector<std::string> statics = staticImages();
    result.insert(result.end(), statics.begin(), statics.end());
    return result;
}

std::vector<std::string> formats::animatedImages()
{
    return {".webp", ".apng", ".gif"};
}

std::vector<std::string> formats::staticImages()
{
    return {".png"};
}

#include "rolechat/filesystem/RCFile.h"
#include "rolechat/util/FileSystem.h"
#include "rolechat/filesystem/RCDir.h"
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <rolechat/userdata/RolechatDatabase.h>

using namespace rolechat::fs;

RCFile::RCFile(const std::string &path, bool allowPackages) : m_allowPackages(allowPackages)
{
    m_fileNameList.push_back(path);
}

RCFile::RCFile(const std::string &path, const std::vector<std::string> &extensions, bool allowPackages) : m_extensions(extensions), m_allowPackages(allowPackages)
{
    m_fileNameList.push_back(path);
}

RCFile::RCFile(const std::vector<std::string> &paths, bool allowPackages) : m_allowPackages(allowPackages), m_fileNameList(paths)       
{
}

RCFile::RCFile(const std::vector<std::string> &paths, const std::vector<std::string> &extensions, bool allowPackages) : m_allowPackages(allowPackages), m_extensions(extensions), m_fileNameList(paths)  
{
}

std::string RCFile::findFirst() const
{
    bool searchingAbsolute = false;
    std::vector<std::string> candidates;
    for (const auto& filePath : m_fileNameList)
    {
        if(!searchingAbsolute && filePath.length() > 3)
          searchingAbsolute = filePath[1] == ':' && (filePath[2] == '/' || filePath[2] == '\\');

        candidates.push_back(filePath);
        for (const auto& ext : m_extensions)
            candidates.push_back(filePath + ext);
    }

    if(searchingAbsolute)
    {
      for (const auto& candidate : candidates)
      {
        if (exists(candidate))
          return candidate;
      }
    }

    if (m_allowPackages && !searchingAbsolute)
    {
        for (const auto& packageName : PackageManager::packageNames()) 
        {
            auto disabled = PackageManager::disabledList();
            if (std::find(disabled.begin(), disabled.end(), packageName) == disabled.end()) 
            {
                for (const auto& candidate : candidates) 
                {
                    std::string packagePath = RCDir::packagePath(packageName) + candidate;
                    if (exists(packagePath))
                        return packagePath;
                }
            }
        }

        for(const auto& mounted : RolechatDatabase::instance().mountedDirectories(true))
        {
          for (const auto& candidate : candidates)
          {
            if (candidate.rfind("theme/", 0) == 0)
              continue;

            std::string packagePath = mounted.directory + "/" + candidate;
            if (exists(packagePath))
              return packagePath;
          }
        }

    }

    for (const auto& candidate : candidates) 
    {
        std::string baseFilePath = RCDir::basePath() + candidate;
        if (exists(baseFilePath))
            return baseFilePath;
    }

    return "";
}

std::vector<std::string> RCFile::findAll() const
{
    std::vector<std::string> results;
    std::vector<std::string> candidates;

    
    for (const auto& filePath : m_fileNameList) 
    {
        candidates.push_back(filePath);
        for (const auto& ext : m_extensions)
            candidates.push_back(filePath + ext);
    }

    // Search in packages
    for (const auto& packageName : PackageManager::packageNames()) 
    {
        auto disabled = PackageManager::disabledList();
        if (std::find(disabled.begin(), disabled.end(), packageName) == disabled.end()) 
        {
            for (const auto& candidate : candidates) 
            {
                std::string packagePath = RCDir::packagePath(packageName) + candidate;
                if (exists(packagePath))
                    results.push_back(packagePath);
            }
        }
    }


    for(const auto& mounted : RolechatDatabase::instance().mountedDirectories(true))
    {
      for (const auto& candidate : candidates)
      {
        if (candidate.rfind("theme/", 0) == 0)
          continue;

        std::string packagePath = mounted.directory + "/" + candidate;
        if (exists(packagePath))
          results.push_back(packagePath);
      }
    }

    for (const auto& candidate : candidates) {
        std::string baseFilePath = RCDir::basePath() + candidate;
        if (exists(baseFilePath))
            results.push_back(baseFilePath);
    }

    return results;
}

bool RCFile::exists(const std::string &path)
{
    std::filesystem::path fsPath = std::filesystem::u8path(path);
    return std::filesystem::exists(fsPath) && std::filesystem::is_regular_file(fsPath);
}

bool RCFile::exists() const
{
    return findFirst().empty() == false;
}

std::string RCFile::caseSensitivePath(const std::string &inputPath)
{
    std::string result = inputPath;
    while (result.find("//") != std::string::npos) {
        result.replace(result.find("//"), 2, "/");
    }
    return result;
}

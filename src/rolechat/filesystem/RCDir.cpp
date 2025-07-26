#include "rolechat/filesystem/RCDir.h"
#include "rolechat/util/FileSystem.h"
#include <filesystem>

using namespace rolechat::fs;

RCDir::RCDir(const std::string &path, bool allowPackages) : m_allowPackages(allowPackages) 
{
    m_filePathList.push_back(path);
}

RCDir::RCDir(const std::vector<std::string> &paths, bool allowPackages) : m_filePathList(paths), m_allowPackages(allowPackages) 
{
}

std::string RCDir::findFirst() const
{
    if (m_allowPackages) 
    {
        for (const auto& packageName : PackageManager::packageNames()) 
        {
            auto disabled = PackageManager::disabledList();
            if (std::find(disabled.begin(), disabled.end(), packageName) == disabled.end()) 
            {
                for (const auto& candidate : m_filePathList) 
                {
                    std::string baseDir = paths::packagePath(packageName) + candidate;
                    if (exists(baseDir)) 
                        return baseDir;
                }
            }
        }
    }

    for (const auto& candidate : m_filePathList) 
    {
        std::string baseDir = paths::basePath() + candidate;
        if (exists(baseDir)) 
            return baseDir;
    }

    return "";
}

std::vector<std::string> RCDir::findAll() const
{
    std::vector<std::string> result;

    if (m_allowPackages) 
    {
        for (const auto& packageName : PackageManager::packageNames()) 
        {
            auto disabled = PackageManager::disabledList();
            if (std::find(disabled.begin(), disabled.end(), packageName) == disabled.end()) 
            {
                for (const auto& candidate : m_filePathList) 
                {
                    std::string baseDir = paths::packagePath(packageName) + candidate;
                    if (!exists(baseDir)) 
                        continue;
                    result.push_back(baseDir);
                }
            }
        }
    }

    for (const auto& candidate : m_filePathList) 
    {
        std::string baseDir = paths::basePath() + candidate;
        if (!exists(baseDir)) 
            continue;
        result.push_back(baseDir);
    }

    return result;
}

bool RCDir::exists(const std::string &path)
{
    return std::filesystem::exists(path) && std::filesystem::is_directory(path);
}

bool RCDir::exists() const
{
    return findFirst().empty() == false;
}

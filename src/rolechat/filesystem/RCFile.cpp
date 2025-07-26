#include "rolechat/filesystem/RCFile.h"
#include "rolechat/util/FileSystem.h"
#include <filesystem>
#include <fstream>

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
    std::vector<std::string> candidates;
    for (const auto& filePath : m_fileNameList) 
    {
        candidates.push_back(filePath);
        for (const auto& ext : m_extensions)
            candidates.push_back(filePath + ext);
    }

    if (m_allowPackages) 
    {
        for (const auto& packageName : PackageManager::packageNames()) 
        {
            auto disabled = PackageManager::disabledList();
            if (std::find(disabled.begin(), disabled.end(), packageName) == disabled.end()) 
            {
                for (const auto& candidate : candidates) 
                {
                    std::string packagePath = paths::packagePath(packageName) + candidate;
                    if (checks::fileExists(packagePath))
                        return packagePath;
                }
            }
        }
    }

    for (const auto& candidate : candidates) 
    {
        std::string baseFilePath = paths::basePath() + candidate;
        if (checks::fileExists(baseFilePath))
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
                std::string packagePath = paths::packagePath(packageName) + candidate;
                if (checks::fileExists(packagePath))
                    results.push_back(packagePath);
            }
        }
    }

    for (const auto& candidate : candidates) {
        std::string baseFilePath = paths::basePath() + candidate;
        if (checks::fileExists(baseFilePath))
            results.push_back(baseFilePath);
    }

    return results;
}

bool RCFile::exists(const std::string &path)
{
    return std::filesystem::exists(path) && std::filesystem::is_regular_file(path);
}

bool RCFile::exists() const
{
    return findFirst().empty() == false;
}

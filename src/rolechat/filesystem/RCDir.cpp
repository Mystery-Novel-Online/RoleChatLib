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
                    std::string baseDir = packagePath(packageName) + candidate;
                    if (exists(baseDir)) 
                        return baseDir;
                }
            }
        }
    }

    for (const auto& candidate : m_filePathList) 
    {
        std::string baseDir = basePath() + candidate;
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
                    std::string baseDir = packagePath(packageName) + candidate;
                    if (!exists(baseDir)) 
                        continue;
                    result.push_back(baseDir);
                }
            }
        }
    }

    for (const auto& candidate : m_filePathList) 
    {
        std::string baseDir = basePath() + candidate;
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

std::vector<std::string> RCDir::subDirectories() const
{
    std::vector<std::string> result;

    std::vector<std::string> directoryList = findAll();
    for (const auto& candidate : directoryList) 
    {
        for (const auto& entry : std::filesystem::directory_iterator(candidate)) 
        {
            if (entry.is_directory())
                result.push_back(entry.path().filename().string());
        }
    }

    return result;
}

std::vector<std::string> RCDir::subDirectories(const std::string &path)
{
    std::vector<std::string> result;

    if(!exists(path))
        return result;
        
    for (const auto& entry : std::filesystem::directory_iterator(path)) 
    {
        if (entry.is_directory())
            result.push_back(entry.path().filename().string());
    }

    return result;
}

std::vector<std::string> RCDir::fileList(const std::string &extensionFilter, bool includeExtension) const
{
    std::vector<std::string> result;
    std::vector<std::string> candidates = findAll();

    for (const auto& candidate : candidates) 
    {
        for (const auto& entry : std::filesystem::directory_iterator(candidate))
        {
            if (entry.is_regular_file()) {
                std::string filename = entry.path().filename().string();
                std::string ext = entry.path().extension().string();
                if (!extensionFilter.empty()) 
                {
                    if (ext.empty() || ext.substr(1) != extensionFilter)
                        continue;
                }

                if (includeExtension)
                    result.push_back(filename);
                else
                    result.push_back(entry.path().stem().string());
                }
            }
    }

    return result;
}

std::vector<std::string> RCDir::fileList(const std::string &directory, const std::string &extensionFilter, bool includeExtension)
{
    std::vector<std::string> result;

    if(!exists(directory))
        return result;

    for (const auto& entry : std::filesystem::directory_iterator(directory))
    {
        if (entry.is_regular_file()) {
            std::string filename = entry.path().filename().string();
            std::string ext = entry.path().extension().string();
            if (!extensionFilter.empty()) 
            {
                if (ext.empty() || ext.substr(1) != extensionFilter)
                    continue;
            }
            if (includeExtension)
                result.push_back(filename);
            else
                result.push_back(entry.path().stem().string());
            }
        }
    

    return result;
}


// Implementation of paths functions
std::string RCDir::applicationPath()
{
    auto path = std::filesystem::current_path();
#if defined(__APPLE__)
    for (int i = 0; i < 3; ++i)
        path = path.parent_path();
#endif
    return path.string();
}

std::string RCDir::basePath()
{
    return applicationPath() + "/base/";
}

std::string RCDir::packagePath(const std::string &packageName)
{
    return applicationPath() + "/packages/" + packageName + "/";
}

#ifndef FILESYSTEM_RCDIR_H
#define FILESYSTEM_RCDIR_H
#include <string>
#include <vector>

namespace rolechat::fs
{
class RCDir
{
public:
    RCDir(const std::string &path, bool allowPackages = true);
    RCDir(const std::vector<std::string> &paths, bool allowPackages = true);

    std::string findFirst() const;
    std::vector<std::string> findAll() const;

    std::vector<std::string> subDirectories() const;
    static std::vector<std::string> subDirectories(const std::string &path);

    std::vector<std::string> fileList(const std::string &extensionFilter = "", bool includeExtension = true) const;
    static std::vector<std::string> fileList(const std::string &directory, const std::string &extensionFilter, bool includeExtension);

    bool exists() const;
    static bool exists(const std::string &path);


    static std::string applicationPath();
    static std::string basePath();
    static std::string packagePath(const std::string& packageName);
private:
    std::vector<std::string> m_filePathList;
    bool m_allowPackages = true;
};
}
#endif
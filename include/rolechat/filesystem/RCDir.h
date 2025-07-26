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

    bool exists() const;
    static bool exists(const std::string &path);
private:
    std::vector<std::string> m_filePathList;
    bool m_allowPackages = true;
};
}
#endif
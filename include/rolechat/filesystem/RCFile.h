#ifndef RCFile_H
#define RCFile_H
#include <string>
#include <vector>

namespace rolechat::fs
{
class RCFile
{
public:
    RCFile(const std::string &path, bool allowPackages = true);
    RCFile(const std::string &path, const std::vector<std::string>& extensions, bool allowPackages = true);

    RCFile(const std::vector<std::string> &paths, bool allowPackages = true);
    RCFile(const std::vector<std::string> &paths, const std::vector<std::string>& extensions, bool allowPackages = true);

    std::string findFirst() const;
    std::vector<std::string> findAll() const;

    bool exists() const;
    static bool exists(const std::string &path);
    static std::string caseSensitivePath(const std::string &path);

private:
    std::vector<std::string> m_fileNameList;
    std::vector<std::string> m_extensions;

    bool m_allowPackages = true;
};
}
#endif
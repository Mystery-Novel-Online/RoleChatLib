#pragma once

#include <string>
#include <sqlite3.h>
#include <vector>

class RolechatDatabase {
public:
    RolechatDatabase();
    ~RolechatDatabase();

    bool initTables();
    bool exec(const std::string& sql);

    bool incrementCharacterUsage(const std::string& character);
    std::vector<std::tuple<std::string, int, long long>> getCharactersSortedByLastUsed();
    std::vector<std::tuple<std::string, int, long long>> getCharactersSortedByUsage();

    std::string workshopGuid(std::string folderName);
    int workshopUpdateTime(std::string folderName);

    bool cacheContentData(const std::string& guid, const std::string& folder, const int lastUpdated, const int contentId);

private:
    sqlite3* db = nullptr;
};

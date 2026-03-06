#pragma once

#include "rolechat/userdata/SQLDB.h"
#include <string>
#include <sqlite3.h>
#include <vector>
#include <mutex>

struct WorkshopData
{

  const std::string downloadUri()
  {
    return "api/workshop/"+ guid +"/content";
  };

  std::string guid;
  std::string folder;
  int lastUpdated;
  int content_id;
};

struct MountedDirectory
{
  std::string directory;
  bool active;
};

class RolechatDatabase : public SQLDB {
public:

    static RolechatDatabase& instance();

    RolechatDatabase();
    ~RolechatDatabase();

    bool initTables() override;

    bool incrementCharacterUsage(const std::string& character);
    std::vector<std::tuple<std::string, int, long long>> getCharactersSortedByLastUsed();
    std::vector<std::tuple<std::string, int, long long>> getCharactersSortedByUsage();


    WorkshopData searchContentGuid(const std::string& guid);

    std::string workshopGuid(std::string folderName);
    int workshopUpdateTime(std::string folderName);

    bool cacheContentData(const std::string& guid, const std::string& folder, const int lastUpdated, const int contentId);


    //Mounted Directories
    void toggleMount(const std::string& path, bool active);
    void removeMount(const std::string& path);
    std::vector<MountedDirectory> mountedDirectories(bool excludeInactive = false);

private:
    std::mutex m_mutex;
};

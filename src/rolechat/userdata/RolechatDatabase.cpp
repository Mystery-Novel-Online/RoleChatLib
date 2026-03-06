#include "rolechat/userdata/RolechatDatabase.h "
#include <iostream>

#include "rolechat/userdata/SQLTable.h"
#include <rolechat/userdata/SQLStmt.h>

static SQLTable CHARACTER_USAGE_TABLE =
    SQLTable("character_usage")
      .text("character").notNull().unique().primaryKey().done()
      .integer("uses").defaultValue(0).done()
      .integer("last_used").defaultValue(0).done();


static SQLTable WORKSHOP_DATA_TABLE =
    SQLTable("workshop_data")
        .text("guid").notNull().unique().primaryKey().done()
        .text("folder").done()
        .integer("last_updated").defaultValue(0).done()
        .integer("content_id").defaultValue(-1).done();

static SQLTable MOUNTED_FOLDERS_TABLE =
    SQLTable("mounted_directories")
        .text("directory").notNull().unique().primaryKey().done()
        .integer("active_state").defaultValue(1).done();


static SQLTable WORKSHOP_BACKGROUNDS_TABLE =
    SQLTable("workshop_backgrounds")
        .text("guid").notNull().unique().primaryKey().done()
        .text("folder").done()
        .integer("last_updated").defaultValue(0).done();


RolechatDatabase::RolechatDatabase() {
    bool success = loadDb("base/configs/user_data.db");
}

RolechatDatabase::~RolechatDatabase() {
  if (db()) {
        sqlite3_close(db());
    }
}

bool RolechatDatabase::initTables() 
{
  std::vector<std::string> statements = {
      CHARACTER_USAGE_TABLE.build(),
      WORKSHOP_DATA_TABLE.build(),
      MOUNTED_FOLDERS_TABLE.build(),
      WORKSHOP_BACKGROUNDS_TABLE.build()
  };

  for (const auto& sql : statements)
  {
    std::cout << sql << std::endl;
    if (!exec(sql))
      return false;
  }

  return true;
}


bool RolechatDatabase::incrementCharacterUsage(const std::string &character)
{
  std::lock_guard<std::mutex> lock(m_mutex);
  if(character.empty())
    return false;

  if (!db()) return false;

  const char* sql = R"(
        INSERT INTO character_usage (character, uses, last_used)
        VALUES (?, 1, strftime('%s', 'now'))
        ON CONFLICT(character) DO UPDATE SET
            uses = uses + 1,
            last_used = strftime('%s', 'now');
    )";

  sqlite3_stmt* stmt = nullptr;
  if (sqlite3_prepare_v2(db(), sql, -1, &stmt, nullptr) != SQLITE_OK)
    return false;

  sqlite3_bind_text(stmt, 1, character.c_str(), -1, SQLITE_TRANSIENT);

  bool ok = (sqlite3_step(stmt) == SQLITE_DONE);
  sqlite3_finalize(stmt);
  return ok;
}

std::vector<std::tuple<std::string, int, long long> > RolechatDatabase::getCharactersSortedByLastUsed()
{
  std::lock_guard<std::mutex> lock(m_mutex);
  std::vector<std::tuple<std::string, int, long long>> results;

  if (!db()) return results;

  const char* sql = R"(
        SELECT character, uses, last_used
        FROM character_usage
        ORDER BY last_used DESC;
    )";

  sqlite3_stmt* stmt = nullptr;
  if (sqlite3_prepare_v2(db(), sql, -1, &stmt, nullptr) != SQLITE_OK)
    return results;

  while (sqlite3_step(stmt) == SQLITE_ROW) {
    std::string character = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    int uses = sqlite3_column_int(stmt, 1);
    long long lastUsed = sqlite3_column_int64(stmt, 2);

    results.emplace_back(character, uses, lastUsed);
  }

  sqlite3_finalize(stmt);
  return results;
}

std::vector<std::tuple<std::string, int, long long> > RolechatDatabase::getCharactersSortedByUsage()
{
  std::lock_guard<std::mutex> lock(m_mutex);
  std::vector<std::tuple<std::string, int, long long>> results;

  if (!db()) return results;

  const char* sql = R"(
        SELECT character, uses, last_used
        FROM character_usage
        ORDER BY uses DESC;
    )";

  sqlite3_stmt* stmt = nullptr;
  if (sqlite3_prepare_v2(db(), sql, -1, &stmt, nullptr) != SQLITE_OK)
    return results;

  while (sqlite3_step(stmt) == SQLITE_ROW) {
    std::string character = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    int uses = sqlite3_column_int(stmt, 1);
    long long lastUsed = sqlite3_column_int64(stmt, 2);

    results.emplace_back(character, uses, lastUsed);
  }

  sqlite3_finalize(stmt);
  return results;
}

bool RolechatDatabase::cacheContentData(const std::string &guid, const std::string &folder, const int lastUpdated, const int contentId)
{
  std::lock_guard<std::mutex> lock(m_mutex);
  if (!db()) return false;

  const char* sql = R"(
        INSERT INTO workshop_data (guid, folder, last_updated, content_id)
        VALUES (?, ?, ?, ?)
        ON CONFLICT(guid) DO UPDATE SET
            folder = excluded.folder,
            last_updated = excluded.last_updated,
            content_id = excluded.content_id;
    )";

  sqlite3_stmt* stmt = nullptr;
  if (sqlite3_prepare_v2(db(), sql, -1, &stmt, nullptr) != SQLITE_OK)
    return false;

  sqlite3_bind_text(stmt, 1, guid.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 2, folder.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_int(stmt, 3, lastUpdated);
  sqlite3_bind_int(stmt, 4, contentId);

  bool ok = (sqlite3_step(stmt) == SQLITE_DONE);
  sqlite3_finalize(stmt);
  return ok;
}

std::string RolechatDatabase::workshopGuid(std::string folderName)
{
  std::lock_guard<std::mutex> lock(m_mutex);
  if (!db()) return "";

  const char* sql = R"(
        SELECT guid
        FROM workshop_data
        WHERE folder = ?;
    )";

  sqlite3_stmt* stmt = nullptr;

  if (sqlite3_prepare_v2(db(), sql, -1, &stmt, nullptr) != SQLITE_OK)
    return "";

  sqlite3_bind_text(stmt, 1, folderName.c_str(), -1, SQLITE_TRANSIENT);

  std::string guid = "";
  while (sqlite3_step(stmt) == SQLITE_ROW) {
    guid = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
  }

  sqlite3_finalize(stmt);
  return guid;
}

int RolechatDatabase::workshopUpdateTime(std::string folderName)
{
  std::lock_guard<std::mutex> lock(m_mutex);
  if (!db()) return 0;

  const char* sql = R"(
        SELECT last_updated
        FROM workshop_data
        WHERE folder = ?;
    )";

  sqlite3_stmt* stmt = nullptr;


  if (sqlite3_prepare_v2(db(), sql, -1, &stmt, nullptr) != SQLITE_OK)
    return 0;
  sqlite3_bind_text(stmt, 1, folderName.c_str(), -1, SQLITE_TRANSIENT);
  int lastUpdated = 0;
  while (sqlite3_step(stmt) == SQLITE_ROW) {
    lastUpdated = sqlite3_column_int64(stmt, 0);
  }

  sqlite3_finalize(stmt);
  return lastUpdated;
}

std::vector<MountedDirectory> RolechatDatabase::mountedDirectories(bool excludeInactive)
{
  std::lock_guard<std::mutex> lock(m_mutex);
  std::vector<MountedDirectory> results;

  SQLSelect select = MOUNTED_FOLDERS_TABLE.select();
  if(excludeInactive)
    select.where("active_state = 1");

  SQLStmt stmt = select.prepare(db());

  while (stmt.step())
    results.push_back({stmt.text(0), stmt.integer(1) == 1});

  return results;
}

RolechatDatabase &RolechatDatabase::instance()
{
  static RolechatDatabase instance;
  return instance;
}

void RolechatDatabase::toggleMount(const std::string &path, bool active)
{
  std::lock_guard<std::mutex> lock(m_mutex);
  SQLStmt stmt(db(), R"(
        INSERT INTO mounted_directories (directory, active_state)
        VALUES (?, ?)
        ON CONFLICT(directory) DO UPDATE SET
            active_state = excluded.active_state
    )");

  stmt.bind(1, path);
  stmt.bind(2, active);
  stmt.step();
}

void RolechatDatabase::removeMount(const std::string &path)
{
  std::lock_guard<std::mutex> lock(m_mutex);
  SQLStmt stmt(db(), R"(
        DELETE FROM mounted_directories
        WHERE directory = ?
    )");

  stmt.bind(1, path);
  stmt.step();
}

WorkshopData RolechatDatabase::searchContentGuid(const std::string &guid)
{
  std::lock_guard<std::mutex> lock(m_mutex);
  SQLStmt stmt = WORKSHOP_DATA_TABLE.select()
      .where("guid = '" + guid + "'")
      .prepare(db());

  if(stmt.step())
    return {stmt.text(0), stmt.text(1), stmt.integer(2), stmt.integer(3)};

  return{};

}

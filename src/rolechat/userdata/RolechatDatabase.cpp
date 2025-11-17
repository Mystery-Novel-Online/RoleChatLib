#include "rolechat/userdata/RolechatDatabase.h "
#include <iostream>

static const char* CREATE_CHARACTER_USAGE_TABLE = R"(
CREATE TABLE IF NOT EXISTS character_usage (
    character TEXT PRIMARY KEY,
    uses INTEGER DEFAULT 0,
    last_used INTEGER DEFAULT 0
);
)";

static const char* CREATE_CACHE_TABLE = R"(
CREATE TABLE IF NOT EXISTS workshop_data (
    guid TEXT PRIMARY KEY,
    folder TEXT,
    last_updated INTEGER DEFAULT 0,
    content_id INTEGER
);
)";

RolechatDatabase::RolechatDatabase() {
    std::string fullPath = "base/configs/user_data.db";
    if (sqlite3_open(fullPath.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Failed to open DB: " << sqlite3_errmsg(db) << "\n";
        db = nullptr;
        return;
    }

    if (!initTables()) {
        std::cerr << "Failed to initialize tables in database.\n";
    }
}

RolechatDatabase::~RolechatDatabase() {
    if (db) {
        sqlite3_close(db);
    }
}

bool RolechatDatabase::initTables() 
{
    if (!exec(CREATE_CHARACTER_USAGE_TABLE)) return false;
    if (!exec(CREATE_CACHE_TABLE)) return false;
    return true;
}

bool RolechatDatabase::exec(const std::string& sql) {
    if (!db) return false;

    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << "\n";
        sqlite3_free(errMsg);
        return false;
    }

    return true;
}

bool RolechatDatabase::incrementCharacterUsage(const std::string &character)
{
  if (!db) return false;

  const char* sql = R"(
        INSERT INTO character_usage (character, uses, last_used)
        VALUES (?, 1, strftime('%s', 'now'))
        ON CONFLICT(character) DO UPDATE SET
            uses = uses + 1,
            last_used = strftime('%s', 'now');
    )";

  sqlite3_stmt* stmt = nullptr;
  if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    return false;

  sqlite3_bind_text(stmt, 1, character.c_str(), -1, SQLITE_TRANSIENT);

  bool ok = (sqlite3_step(stmt) == SQLITE_DONE);
  sqlite3_finalize(stmt);
  return ok;
}

std::vector<std::tuple<std::string, int, long long> > RolechatDatabase::getCharactersSortedByLastUsed()
{
  std::vector<std::tuple<std::string, int, long long>> results;

  if (!db) return results;

  const char* sql = R"(
        SELECT character, uses, last_used
        FROM character_usage
        ORDER BY last_used DESC;
    )";

  sqlite3_stmt* stmt = nullptr;
  if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
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
  std::vector<std::tuple<std::string, int, long long>> results;

  if (!db) return results;

  const char* sql = R"(
        SELECT character, uses, last_used
        FROM character_usage
        ORDER BY uses DESC;
    )";

  sqlite3_stmt* stmt = nullptr;
  if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
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
  if (!db) return false;

  const char* sql = R"(
        INSERT INTO workshop_data (guid, folder, last_updated, content_id)
        VALUES (?, ?, ?, ?)
        ON CONFLICT(guid) DO UPDATE SET
            folder = excluded.folder,
            last_updated = excluded.last_updated,
            content_id = excluded.content_id;
    )";

  sqlite3_stmt* stmt = nullptr;
  if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    return false;

  sqlite3_bind_text(stmt, 1, guid.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 2, folder.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_int(stmt, 3, lastUpdated);
  sqlite3_bind_int(stmt, 4, contentId);

  bool ok = (sqlite3_step(stmt) == SQLITE_DONE);
  sqlite3_finalize(stmt);
  return ok;
}

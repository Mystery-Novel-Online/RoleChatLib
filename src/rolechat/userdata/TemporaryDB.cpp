#include "TemporaryDB.h"

#include <rolechat/userdata/SQLTable.h>

#include <rolechat/filesystem/RCDir.h>


static SQLTable AVALIABLE_CHARACTERS_TABLE =
    SQLTable("avaliable_characters")
        .text("character").notNull().unique().primaryKey().done()
        .text("path").unique().done();


TemporaryDB &TemporaryDB::instance()
{
  static TemporaryDB instance;
  return instance;
}

TemporaryDB::TemporaryDB()
{
  loadDb(":memory:");
}

bool TemporaryDB::initTables()
{
  std::vector<std::string> statements = {
      AVALIABLE_CHARACTERS_TABLE.build()
  };

  for (const auto& sql : statements)
  {
    std::cout << sql << std::endl;
    if (!exec(sql))
      return false;
  }

  exportDebug();
  return true;
}

bool TemporaryDB::exportDebug()
{
  sqlite3* fileDb = nullptr;

  if (sqlite3_open("base/debug/memory_db_dump.db", &fileDb) != SQLITE_OK) {
    std::cerr << "Failed to open debug file\n";
    return false;
  }

  sqlite3_backup* backup = sqlite3_backup_init(
      fileDb, "main",
      db(), "main"
      );

  if (!backup) {
    std::cerr << "Backup init failed\n";
    sqlite3_close(fileDb);
    return false;
  }

  sqlite3_backup_step(backup, -1); // copy entire DB
  sqlite3_backup_finish(backup);

  if (sqlite3_errcode(fileDb) != SQLITE_OK) {
    std::cerr << "Backup failed\n";
    sqlite3_close(fileDb);
    return false;
  }

  sqlite3_close(fileDb);

  std::cout << "Memory DB exported to " << "base/debug/memory_db_dump.db" << "\n";
  return true;
}

void TemporaryDB::scanCharacters()
{
  std::lock_guard<std::mutex> lock(m_mutex);
  SQLStmt deleteStmt(db(), "DELETE FROM avaliable_characters");
  deleteStmt.step();

  for(const auto& path : rolechat::fs::RCDir("characters").findAll())
  {
    for (const auto& entry : rolechat::fs::RCDir::subDirectories(path))
    {
      foundCharacter(entry, path + "/" + entry);
    }
  }

  exportDebug();
}

std::vector<std::string> TemporaryDB::avaliableCharacters()
{
  std::lock_guard<std::mutex> lock(m_mutex);
  std::vector<std::string> results;

  SQLStmt stmt = AVALIABLE_CHARACTERS_TABLE.select()
                     .prepare(db());

  while(stmt.step())
    results.push_back(stmt.text(0));

  return results;
}

std::string TemporaryDB::characterPath(const std::string &character)
{
  std::lock_guard<std::mutex> lock(m_mutex);
  SQLStmt stmt = AVALIABLE_CHARACTERS_TABLE.select()
                     .where("character", character)
                     .prepare(db());

  if(stmt.step())
    return stmt.text(1);

  return "";
}

void TemporaryDB::foundCharacter(const std::string &name, const std::string &path)
{
  SQLStmt stmt(db(), R"(
        INSERT INTO avaliable_characters (character, path)
        VALUES (?, ?)
        ON CONFLICT(character) DO NOTHING;
    )");

  stmt.bind(1, name);
  stmt.bind(2, path);
  stmt.step();
}

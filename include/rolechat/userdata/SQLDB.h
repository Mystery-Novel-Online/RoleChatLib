#ifndef SQLDB_H
#define SQLDB_H

#include <iostream>
#include <string>
#include <sqlite3.h>
#include <vector>
#include <mutex>

class SQLDB
{
public:
  ~SQLDB();

  bool loadDb(const std::string& path);
  virtual bool initTables() { return true; };
  bool exec(const std::string& sql);

  sqlite3* db() {return m_db;}

private:
  sqlite3* m_db = nullptr;

};

inline bool SQLDB::exec(const std::string &sql)
{
  if (!m_db) return false;

  char* errMsg = nullptr;
  int rc = sqlite3_exec(m_db, sql.c_str(), nullptr, nullptr, &errMsg);

  if (rc != SQLITE_OK) {
    std::cerr << "SQL error: " << errMsg << "\n";
    sqlite3_free(errMsg);
    return false;
  }

  return true;
}

inline SQLDB::~SQLDB()
{
  if (m_db) {
    sqlite3_close(m_db);
  }
}

inline bool SQLDB::loadDb(const std::string &path)
{
  if (sqlite3_open(path.c_str(), &m_db) != SQLITE_OK) {
    std::cerr << "Failed to open DB: " << sqlite3_errmsg(m_db) << "\n";
    m_db = nullptr;
    return false;
  }

  if (!initTables()) {
    std::cerr << "Failed to initialize tables in database.\n";
    return false;
  }
  return true;
}

#endif // SQLDB_H

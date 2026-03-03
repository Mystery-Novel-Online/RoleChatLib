#ifndef SQLSTMT_H
#define SQLSTMT_H

#include <sqlite3.h>
#include <string>

class SQLStmt {
public:
  sqlite3_stmt* stmt = nullptr;
  SQLStmt(sqlite3* db, const char* sql) { sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr); }

  ~SQLStmt() { if (stmt) sqlite3_finalize(stmt); }

  operator sqlite3_stmt*() { return stmt; }

  bool step() { return sqlite3_step(stmt) == SQLITE_ROW; }
  bool done() { return sqlite3_step(stmt) == SQLITE_DONE; }
  bool success() { return sqlite3_step(stmt) == SQLITE_DONE; }


  void bind(int index, int value) { sqlite3_bind_int(stmt, index, value); }
  void bind(int index, std::string value) { sqlite3_bind_text(stmt, index, value.c_str(), -1, SQLITE_TRANSIENT); }

  std::string text(int col) const {
    const unsigned char* t = sqlite3_column_text(stmt, col);
    return t ? reinterpret_cast<const char*>(t) : "";
  }

  int integer(int col)
  {
    return sqlite3_column_int(stmt, col);
  }
};

#endif // SQLSTMT_H

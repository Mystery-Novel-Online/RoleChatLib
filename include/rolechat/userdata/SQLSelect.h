#ifndef SQLSELECT_H
#define SQLSELECT_H
#include <iostream>
#include <sqlite3.h>
#include <sstream>
#include <string>
#include <vector>

#include "SQLStmt.h"

class SQLSelect {
public:
  explicit SQLSelect(std::string table) : m_table(std::move(table)) {}

  SQLSelect& where(std::string condition) {
    m_whereClause = condition;
    return *this;
  }

  SQLSelect& columns(std::vector<std::string> columns) {
    m_columns = columns;
    return *this;
  }

  SQLSelect& join(const std::string& table, const std::string& on) {
    m_joins.push_back("JOIN " + table + " ON " + on);
    return *this;
  }

  SQLSelect& leftJoin(const std::string& table, const std::string& on) {
    m_joins.push_back("LEFT JOIN " + table + " ON " + on);
    return *this;
  }

  SQLSelect& order(const std::string& order) {
    m_orderClause = order;
    return *this;
  }

  SQLSelect& groupBy(const std::string& clause) {
    m_groupBy = clause;
    return *this;
  }

  SQLStmt prepare(sqlite3* db) const {
    std::stringstream ss;
    ss << "SELECT ";
    if (m_columns.empty()) ss << "*";
    else {
      size_t i = 0;
      for (auto& c : m_columns) {
        ss << c;
        if (i++ != m_columns.size() - 1 ) ss << ", ";
      }
    }

    ss << " FROM " << m_table;


    for (const auto& join : m_joins)
      ss << " " << join;

    if (!m_whereClause.empty())
      ss << " WHERE " << m_whereClause;

    if (!m_groupBy.empty())
      ss << " GROUP BY " << m_groupBy;

    if (!m_orderClause.empty())
      ss << " ORDER BY " << m_orderClause;

    ss << ";";
    std::string sql = ss.str();
    return SQLStmt(db, sql.c_str());
  }

private:
  std::string m_table;
  std::vector<std::string> m_avaliableColumns;
  std::vector<std::string> m_columns;
  std::string m_whereClause;
  std::string m_orderClause;
  std::string m_groupBy;
  std::vector<std::string> m_joins;
};

#endif // SQLSELECT_H

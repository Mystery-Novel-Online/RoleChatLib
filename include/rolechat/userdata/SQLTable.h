#ifndef SQLTABLE_H
#define SQLTABLE_H
#include <string>
#include <vector>
#include <sstream>

#include "rolechat/userdata/SQLColumn.h"
#include "rolechat/userdata/SQLSelect.h"

class SQLTable {
public:
  explicit SQLTable(const std::string& name) : m_name(std::move(name)) {}

  SQLColumn& column(const std::string& name, const std::string& type) {
    m_columns.emplace_back(this, name, type);
    return m_columns.back();
  }

  SQLColumn& integer(const std::string& name) {
    return column(name, "INTEGER");
  }

  SQLColumn& text(const std::string& name) {
    return column(name, "TEXT");
  }

  SQLTable& id() {
    m_columns.emplace_back(this, "id", "INTEGER");
    m_columns.back().primaryKey();
    return *this;
  }

  SQLTable& unique(const std::vector<std::string>& cols) {
    m_uniqueConstraints.push_back(cols);
    return *this;
  }


  std::vector<std::string> columnNames() const {
    std::vector<std::string> names;
    for (auto& col : m_columns)
    {
      names.push_back(col.name());
    }
    return names;
  }


  SQLSelect select() const {
    SQLSelect sel(m_name);
    sel.columns(columnNames()); // default: select all columns
    return sel;
  }

  std::string build() const {
    std::stringstream ss;
    ss << "CREATE TABLE IF NOT EXISTS " << m_name << " (\n";

    for (size_t i = 0; i < m_columns.size(); i++) {
      ss << "    " << m_columns[i].str();
      if (i != m_columns.size() - 1) ss << ",";
      else if (m_uniqueConstraints.size() > 0) ss << ",";
      ss << "\n";
    }

    for (size_t i = 0; i < m_uniqueConstraints.size(); i++) {
      ss << "    UNIQUE(";
      for (size_t j = 0; j < m_uniqueConstraints[i].size(); j++) {
        ss << m_uniqueConstraints[i][j];
        if (j != m_uniqueConstraints[i].size() - 1) ss << ", ";
      }
      ss << ")";
      if (i != m_uniqueConstraints.size() - 1) ss << ",";
      ss << "\n";
    }

    ss << ");";

    return ss.str();
  }

private:
  std::string m_name;
  std::vector<SQLColumn> m_columns;

  std::vector<std::vector<std::string>> m_uniqueConstraints;
};

#endif // SQLTABLE_H

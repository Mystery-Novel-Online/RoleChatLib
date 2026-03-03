#ifndef SQLCOLUMN_H
#define SQLCOLUMN_H
#include <string>

class SQLTable;

class SQLColumn {
public:
  SQLColumn(SQLTable* table, std::string name, std::string type) : m_table(table), m_name(std::move(name)), m_type(std::move(type)) {}

  SQLColumn& notNull() {m_constraints += " NOT NULL"; return *this; }
  SQLColumn& primaryKey() {m_constraints += " PRIMARY KEY"; return *this; }
  SQLColumn& unique() {m_constraints += " UNIQUE"; return *this; }
  SQLColumn& autoIncrement() {m_constraints += " AUTOINCREMENT"; return *this; }

  SQLColumn& defaultValue(const std::string& value) {
    m_constraints += " DEFAULT '" + value + "'";
    return *this;
  }

  SQLColumn& defaultValue(const int value) {
    m_constraints += " DEFAULT " + std::to_string(value);
    return *this;
  }

  SQLTable& done() {return *m_table;}

  std::string str() const {
    return m_name + " " + m_type + m_constraints;
  }

private:
  SQLTable* m_table;
  std::string m_name;
  std::string m_type;
  std::string m_constraints;
};



#endif // SQLCOLUMN_H

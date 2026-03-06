#ifndef TEMPORARYDB_H
#define TEMPORARYDB_H

#include <rolechat/userdata/SQLDB.h>


class TemporaryDB : public SQLDB
{
public:

  static TemporaryDB& instance();

  TemporaryDB();
  bool initTables() override;
  bool exportDebug();

  void scanCharacters();
  std::vector<std::string> avaliableCharacters();
  std::string characterPath(const std::string& character);

private:
  void foundCharacter(const std::string& name, const std::string& path);

  std::mutex m_mutex;
};

#endif // TEMPORARYDB_H

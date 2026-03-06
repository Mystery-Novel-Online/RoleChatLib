#ifndef TEMPORARYDB_H
#define TEMPORARYDB_H

#include <rolechat/userdata/SQLDB.h>


class TemporaryDB : public SQLDB
{
public:
  TemporaryDB();

  bool initTables() override;
};

#endif // TEMPORARYDB_H

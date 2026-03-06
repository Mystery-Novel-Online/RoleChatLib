#include "TemporaryDB.h"

TemporaryDB::TemporaryDB()
{
  loadDb(":memory:");
}

bool TemporaryDB::initTables()
{

}

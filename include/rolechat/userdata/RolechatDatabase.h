#pragma once

#include <string>
#include <sqlite3.h>

class RolechatDatabase {
public:
    RolechatDatabase(const std::string& path);
    ~RolechatDatabase();

    bool exec(const std::string& sql);

private:
    sqlite3* db = nullptr;
};

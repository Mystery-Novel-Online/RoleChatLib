#include "rolechat/userdata/database.h "
#include <iostream>

RolechatDatabase::RolechatDatabase(const std::string& path) {
    std::string fullPath = "base/configs/" + path;
    if (sqlite3_open(fullPath.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Failed to open DB: " << sqlite3_errmsg(db) << "\n";
        db = nullptr;
    }
}

RolechatDatabase::~RolechatDatabase() {
    if (db) {
        sqlite3_close(db);
    }
}

bool RolechatDatabase::exec(const std::string& sql) {
    if (!db) return false;

    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << "\n";
        sqlite3_free(errMsg);
        return false;
    }

    return true;
}

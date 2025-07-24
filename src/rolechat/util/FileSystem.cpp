#include "rolechat/util/FileSystem.h"
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace rolechat::fs;
bool checks::fileExists(const std::string &path)
{
    return std::filesystem::exists(path) && std::filesystem::is_regular_file(path);
}

bool checks::directoryExists(const std::string &path)
{
    return std::filesystem::exists(path) && std::filesystem::is_directory(path);
}

bool checks::characterExists(const std::string &path)
{
    return false;
    //return directoryExists(Paths::FindDirectory("characters/" + characterFolder));
}
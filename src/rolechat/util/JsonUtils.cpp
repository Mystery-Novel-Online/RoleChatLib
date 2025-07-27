#include "rolechat/util/JsonUtils.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <regex>

using namespace rolechat::JsonUtils;

JsonData rolechat::JsonUtils::loadFile(const std::string &path)
{
    bool validJson = false;
    return loadFile(path, validJson);
}

JsonData rolechat::JsonUtils::loadFile(const std::string &path, bool &validJson)
{
    std::ifstream file(path);
    if (!file) {
        std::cerr << "Failed to open file: " << path << "\n";
        validJson = false;
        return {};
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    file.close();

    static std::regex trailingCommas(R"(,\s*([\]}]))");
    content = std::regex_replace(content, trailingCommas, "$1");

    try 
    {
        JsonData parsed = JsonData::parse(content);
        validJson = true;
        return parsed;
    } catch (const std::exception& e) 
    {
        validJson = false;
        return {};
    }

}
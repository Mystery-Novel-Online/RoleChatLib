#pragma once
#include <string>

namespace rolechat::fs::checks
{
  bool fileExists(const std::string& path);
  bool directoryExists(const std::string& path);
  bool characterExists(const std::string& character);
}

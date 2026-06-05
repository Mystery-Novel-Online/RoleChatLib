#pragma once

#include <string>

namespace rolechat::theme {
struct ThemeTextColor {
  std::string name{};
  std::string code{};
  std::string highlightChars{};
  bool keepCharacters{false};
};

}

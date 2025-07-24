#pragma once
#include <string>

namespace rolechat::actor {

struct ActorScalingPreset
{
  std::string name = "Preset";
  int verticalAlign = 0;
  int scale = 1000;
};
}
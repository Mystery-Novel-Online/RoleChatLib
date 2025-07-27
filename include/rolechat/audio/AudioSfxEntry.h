#ifndef ROLECHAT_AUDIO_AUDIO_SFX_ENTRY_H
#define ROLECHAT_AUDIO_AUDIO_SFX_ENTRY_H
#include <string>
namespace rolechat::audio {
struct AudioSfxEntry
{
public:
  AudioSfxEntry() = default;
  //TODO: For consistancy, name and file should be trimmed.
  AudioSfxEntry(const std::string& name, const std::string& file, bool foundState = false)
      : name(name), file(file), is_found(foundState)
  {}

  std::string name;
  std::string file;
  bool is_found;
};
}
#endif
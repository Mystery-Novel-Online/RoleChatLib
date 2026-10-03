#include "rolechat/actor/JsonActorData.h"
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <rolechat/filesystem/RCDir.h>

using namespace rolechat::actor;

void JsonActorData::load(const std::string &folder, const std::string& path)
{
    setFolder(folder);
    setPath(path);

    JsonData jsonData = JsonUtils::loadFile(path + "/char.json", m_validCharacter);

    if(!m_validCharacter) {
      return;
    }

    setShowname(jsonData.value("showname", ""));
    setGender(jsonData.value("gender", ""));
    setBlips(jsonData.value("blips", ""));
    setSide(jsonData.value("side", ""));

    m_outfitsOrder.clear();
    std::vector<std::string> include = {};


    if (jsonData.contains("include") && jsonData["include"].is_array()) {
      for (const auto& val : jsonData["include"]) {
        if (val.is_string()) {
          std::string includeName = val.get<std::string>();
          include.push_back(includeName);
        }
      }
    }
    else
    {
      std::string includeName = jsonData.value("include", "");
      if(!includeName.empty()) {
        include.push_back(includeName);
      }
    }

    for(auto& includeName : include) {
        rolechat::fs::RCDir directory("characters/" + includeName, true);
        std::string result = directory.findFirst();
        if(!result.empty()) {
          m_includedActorData[includeName] = std::make_unique<rolechat::actor::JsonActorData>();
          m_includedActorData[includeName]->load(includeName, result);
          m_outfitsOrder = outfitNames();
        }
    }

    setScalingMode(jsonData.value("scaling_mode", "automatic"));

    if (jsonData.contains("outfit_order") && jsonData["outfit_order"].is_array()) 
    {
      for (const auto& val : jsonData["outfit_order"])
      {
        if (val.is_string()) {
          std::string outfitName = val.get<std::string>();
          m_outfitsOrder.push_back(outfitName);
        }
      }
    }

    std::vector<ActorScalingPreset> presets;
    if (jsonData.contains("scaling_presets") && jsonData["scaling_presets"].is_array()) {
        for (const auto& presetValue : jsonData["scaling_presets"]) {
            if (presetValue.is_object()) {
                const auto& obj = presetValue;
                if (obj.contains("name")) {
                    ActorScalingPreset preset;
                    preset.name = obj["name"].get<std::string>();
                    if (obj.contains("horizontal")) {
                      preset.horizontalAlign = obj["horizontal"].get<int>();
                    }
                    if (obj.contains("vertical"))
                        preset.verticalAlign = obj["vertical"].get<int>();
                    if (obj.contains("scale"))
                        preset.scale = obj["scale"].get<int>();
                    presets.push_back(preset);
                }
            }
        }
    }
    setScalingPresets(presets);

    reload();
}

void JsonActorData::reload()
{
    namespace fs = std::filesystem;
    m_outfitNames.clear();

    std::string actorPath = path();
    if(!m_validCharacter) {
      return;
    }

    std::filesystem::path outfitPath = std::filesystem::u8path(actorPath + "/outfits");

    // List subdirectories in outfitPath
    std::vector<std::string> subdirs;
    if (std::filesystem::exists(outfitPath) && std::filesystem::is_directory(outfitPath)) {
      for (const auto& entry : fs::directory_iterator(outfitPath)) {
        if (entry.is_directory()) {
          subdirs.push_back(entry.path().filename().string());
        }
      }
    }
    if (subdirs.empty()) {
      return;
    }


    for (const auto& [actorName, actor] : m_includedActorData) {
      for (const auto& [name, outfit] : actor->outfits()) {
        if (m_outfits.find(name) != m_outfits.end()) {
          m_outfits[name]->mergeOutfit(*outfit);
          continue;
        }
        m_outfits[name] = std::make_unique<actor::ActorOutfit>(*outfit);
        m_outfitNames.push_back(name);
      }
    }


    for (const std::string& name : subdirs) {
        std::filesystem::path fullOutfitPath = std::filesystem::u8path(actorPath + "/outfits/" + name + "/outfit.json");
        std::time_t modifiedTime = 0;
        try {
          modifiedTime = fs::last_write_time(fs::path(fullOutfitPath)).time_since_epoch().count();
        } catch (...) {
          continue;
        }

        bool needsReload = true;

        auto outfitIt = m_outfits.find(name);
        auto modTimeIt = m_outfitModifiedTimes.find(name);

        if (outfitIt != m_outfits.end() && modTimeIt != m_outfitModifiedTimes.end())
        {
          if (modTimeIt->second == modifiedTime)
          {
            needsReload = false;
          }
          else
          {
            m_outfits.erase(outfitIt);
            m_outfitModifiedTimes.erase(modTimeIt);
          }
        }

        if (needsReload)
        {
          m_outfitNames.push_back(name);
          if (m_outfits.find(name) != m_outfits.end()) {
            m_outfits[name]->mergeOutfit(rolechat::actor::ActorOutfit(folder(), name, actorPath));
          }
          else {
            m_outfits[name] = std::make_unique<rolechat::actor::ActorOutfit>(folder(), name, actorPath);
          }
          m_outfitModifiedTimes[name] = modifiedTime;
        }
        else
        {
          m_outfitNames.push_back(name);
        }
    }

    std::vector<std::string> includedOutfits = {};
    if(!m_includedActorData.empty()) {
      for (const auto& [actorName, actor] : m_includedActorData) {
        for (const auto& outfitName: actor->outfitNames()) {
          includedOutfits.push_back(outfitName);
        }
      }
    }

    std::vector<std::string> ordered;
    for (const auto& name : m_outfitsOrder) {
      if (std::find(includedOutfits.begin(), includedOutfits.end(), name) != includedOutfits.end()) {
        ordered.push_back(name);
      }
      else if (std::find(m_outfitNames.begin(), m_outfitNames.end(), name) != m_outfitNames.end()) {
        ordered.push_back(name);
      }


    }

    for (const auto& name : m_outfitNames) {
      if (std::find(m_outfitsOrder.begin(), m_outfitsOrder.end(), name) == m_outfitsOrder.end()) {
        ordered.push_back(name);
      }
    }



    m_outfitNames = std::move(ordered);
}

std::unordered_map<std::string, ActorOutfit *> JsonActorData::outfits() const
{
  std::unordered_map<std::string, actor::ActorOutfit*> result;

  for (const auto& [name, outfit] : m_outfits) {
    result.emplace(name, outfit.get());
  }

  return result;
}

std::string JsonActorData::showname() const
{
    const std::string& currentOutfit = outfit();
    auto it = m_outfits.find(currentOutfit);
    if (it == m_outfits.end() || !it->second) {
      return IActorData::showname();
    }
    const std::string& outfitShowname = it->second->showname();
    return outfitShowname.empty() ? IActorData::showname() : outfitShowname;
}

std::string JsonActorData::side() const
{
  const std::string& currentOutfit = outfit();
  auto it = m_outfits.find(currentOutfit);
  if (it == m_outfits.end() || !it->second) {
    return IActorData::side();
  }
  return it->second->position().has_value() ? it->second->position().value() : IActorData::side();
}

std::vector<ActorEmote> JsonActorData::emotes()
{
    const std::string& currentOutfit = outfit();

    if (currentOutfit == "<All>")
    {
      std::vector<ActorEmote> all;
      for (const auto& outfitName : m_outfitNames) {
        auto it = m_outfits.find(outfitName);
        if (it != m_outfits.end() && it->second) {
          const auto& emotesVec = it->second->emotes();
          all.insert(all.end(), emotesVec.begin(), emotesVec.end());
        }
      }
      return all;
    }

    auto it = m_outfits.find(currentOutfit);
    if (it != m_outfits.end() && it->second) {
      return it->second->emotes();
    }
    return {};
}

std::string JsonActorData::buttonImage(const ActorEmote& emote, bool enabled) const
{
  return "outfits/" + emote.outfitName + "/emotions/" + emote.comment + (enabled ? "_on" : "");
}

std::string JsonActorData::selectedImage(const ActorEmote& emote) const
{
  return "outfits/" + outfit() + "/emotions/selected";
}

void JsonActorData::switchOutfit(const std::string& outfit)
{
  if (std::find(m_outfitNames.begin(), m_outfitNames.end(), outfit) != m_outfitNames.end() || outfit == "<All>") {
    IActorData::switchOutfit(outfit);
  }
}
std::vector<std::string> rolechat::actor::JsonActorData::outfitNames() const
{
  return m_outfitNames;
}

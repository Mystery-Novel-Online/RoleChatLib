#include "rolechat/actor/JsonActorData.h"
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <chrono>

using namespace rolechat::actor;

void JsonActorData::load(const std::string &folder, const std::string& path)
{
    setFolder(folder);
    setPath(path);

    bool validJson = false;
    JsonData jsonData = JsonUtils::loadFile(path + "/char.json", validJson); 

    setShowname(jsonData.value("showname", ""));
    setGender(jsonData.value("gender", ""));
    setSide(jsonData.value("side", ""));

    setScalingMode(jsonData.value("scaling_mode", "automatic"));

    m_outfitsOrder.clear();
    if (jsonData.contains("outfit_order") && jsonData["outfit_order"].is_array()) 
    {
        for (const auto& val : jsonData["outfit_order"]) 
        {
            if (val.is_string())
                m_outfitsOrder.push_back(val.get<std::string>());
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
    std::string outfitPath = actorPath + "/outfits";

    // List subdirectories in outfitPath
    std::vector<std::string> subdirs;
    for (const auto& entry : fs::directory_iterator(outfitPath)) {
        if (entry.is_directory()) {
            subdirs.push_back(entry.path().filename().string());
        }
    }
    if (subdirs.empty()) return;

    for (const std::string& name : subdirs)
    {
        std::string fullOutfitPath = outfitPath + "/" + name + "/outfit.json";
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
                delete outfitIt->second;
                m_outfits.erase(outfitIt);
                m_outfitModifiedTimes.erase(modTimeIt);
            }
        }

        if (needsReload)
        {
            m_outfitNames.push_back(name);
            m_outfits[name] = new rolechat::actor::ActorOutfit(folder(), name, actorPath);
            m_outfitModifiedTimes[name] = modifiedTime;
        }
        else
        {
            m_outfitNames.push_back(name);  
        }
    }

    std::vector<std::string> ordered;
    for (const auto& name : m_outfitsOrder)
        if (std::find(m_outfitNames.begin(), m_outfitNames.end(), name) != m_outfitNames.end())
            ordered.push_back(name);

    for (const auto& name : m_outfitNames)
        if (std::find(m_outfitsOrder.begin(), m_outfitsOrder.end(), name) == m_outfitsOrder.end())
            ordered.push_back(name);

    m_outfitNames = std::move(ordered);
}

std::string JsonActorData::showname() const
{
    const std::string& currentOutfit = outfit();
    auto it = m_outfits.find(currentOutfit);
    if (it == m_outfits.end() || !it->second) return IActorData::showname();
    const std::string& outfitShowname = it->second->showname();
    return outfitShowname.empty() ? IActorData::showname() : outfitShowname;
}

std::vector<ActorEmote> JsonActorData::emotes() const
{
    const std::string& currentOutfit = outfit();

    if (currentOutfit == "<All>")
    {
        std::vector<ActorEmote> all;
        for (const auto& outfitName : m_outfitNames)
        {
            auto it = m_outfits.find(outfitName);
            if (it != m_outfits.end() && it->second)
            {
                const auto& emotesVec = it->second->emotes();
                all.insert(all.end(), emotesVec.begin(), emotesVec.end());
            }
        }
        return all;
    }

    auto it = m_outfits.find(currentOutfit);
    if (it != m_outfits.end() && it->second)
    {
        return it->second->emotes();
    }
    return {};
}

std::string JsonActorData::buttonImage(const ActorEmote& emote, bool enabled) const
{
    return "outfits/" + emote.outfitName + "/emotions/" + emote.comment + (enabled ? "_on" : "") + ".png";
}

std::string JsonActorData::selectedImage(const ActorEmote& emote) const
{
    return "outfits/" + outfit() + "/emotions/selected.png";
}

void JsonActorData::switchOutfit(const std::string& outfit)
{
    if (std::find(m_outfitNames.begin(), m_outfitNames.end(), outfit) != m_outfitNames.end() || outfit == "<All>")
        IActorData::switchOutfit(outfit);
}

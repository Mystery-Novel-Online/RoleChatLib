#include "rolechat/actor/ActorOutfit.h"
#include "rolechat/util/FileSystem.h"
#include "rolechat/filesystem/RCFile.h"

using namespace rolechat::actor;

ActorOutfit::ActorOutfit(const std::string &character, const std::string &outfit, const std::string &characterPath) : m_name(outfit), m_character(character)
{
    m_path = characterPath + "/outfits/" + outfit;
    std::string jsonPath = m_path + "/outfit.json";

    if(!fs::RCFile::exists(jsonPath))
        return;

    bool validJson = false;
    JsonUtils::JsonData jsonData = JsonUtils::loadFile(jsonPath, validJson);

    if(!validJson)
        return;
    
    m_showname = jsonData.value("showname", "");

    m_showDesk = jsonData["default_rules"].value("show_desk", true);
    m_ignoreOffsets = jsonData["default_rules"].value("ignore_offsets", false);

    for (const auto& overlayData : jsonData["layers"]) 
    {
        if (!overlayData.is_object()) continue;

        ActorLayer layer;
        const auto& obj = overlayData;

        layer.offsetName = obj.value("name", "");
        layer.spriteOrder = obj.value("order", "");
        layer.blendMode = obj.value("blend_mode", "");

        if(obj.contains("offset"))
        {
          layer.layerOffset =
          {
              obj["offset"].value("x", 0),
              obj["offset"].value("y", 0),
              obj["offset"].value("width", 0),
              obj["offset"].value("height", 0)
          };
        }

        
        layer.variationOptions.clear();
        std::string imagePrefix = layer.offsetName == "base_image" ? "outfits/" + outfit + "/" : "";
        if (obj.contains("variations") && obj["variations"].is_array()) 
        {
            for (const auto& val : obj["variations"]) 
            {
                if (val.is_string())
                    layer.variationOptions.push_back(imagePrefix + val.get<std::string>());
            }
        }

        layer.globalSelection = obj.value("selection_global", false);
        layer.defaultDisabled = obj.value("toggle_disabled", false);
        layer.toggleName = obj.value("toggle", "");

        m_layers.push_back(layer);
    }

    readEmotes(jsonData);
}

void ActorOutfit::readEmotes(const JsonData& data)
{
    for (const auto& emoteData : data["emotes"]) 
    {
        if (!emoteData.is_object()) continue;

        std::string sharedOutfit = emoteData.contains("outfit") ? emoteData.value("outfit", m_name) : m_name;
        std::string emoteName    = emoteData.value("name", "");
        std::string animName     = emoteData.value("pre", "");
        std::string videoFile    = emoteData.value("video", "");
        std::string soundFile    = emoteData.value("sfx", "");
        std::string sequenceFile = emoteData.value("sequence", "");
        int sfxDelayMs           = emoteData.value("sfx_delay", 0);
        int sfxDelayTicks        = emoteData.value("sfx_delay_ticks", 0);

        std::string outfitPath = sharedOutfit.empty() ? "" : "outfits/" + sharedOutfit + "/";

        ActorEmote emote;
        emote.character     = m_character;
        emote.outfitName    = m_name;
        emote.emoteName     = emoteName;
        emote.comment       = emoteName;
        emote.sequence      = sequenceFile;
        emote.anim          = animName.empty() ? "" : outfitPath + animName;
        emote.dialog        = outfitPath + emoteName;

        if (emoteData.contains("image")) {
            std::string imageOverride = emoteData.value("image", "");
            if (!imageOverride.empty()) {
                emote.dialog = outfitPath + imageOverride;
            }
        }

        // Optional booleans with fallback
        emote.desk_modifier   = emoteData.contains("desk") ? emoteData.value("desk", false) : m_showDesk;
        emote.ignore_offsets  = emoteData.contains("ignore_offsets") ? emoteData.value("ignore_offsets", false) : m_ignoreOffsets;

        emote.modifier        = 0;
        emote.sound_file      = soundFile;
        emote.sound_delay     = (sfxDelayTicks == 0) ? sfxDelayMs : sfxDelayTicks * 60;
        emote.sound_delay     = std::max(0, emote.sound_delay);
        emote.video_file      = videoFile;

        for (const ActorLayer& layer : m_layers)
        {
          if(layer.offsetName == "base_image")
          {
            ActorLayer newLayer = layer;
            newLayer.spriteName = emote.dialog;
            emote.emoteOverlays.push_back(newLayer);
          }
          else
          {
            auto it = emoteData.find(layer.offsetName);
            if (it != emoteData.end() && it->is_string()) {
              std::string overlayImage = it->get<std::string>();
              if (!overlayImage.empty()) {
                ActorLayer newLayer = layer;
                newLayer.spriteName = overlayImage;
                emote.emoteOverlays.push_back(newLayer);
              }
            }
          }
        }

        m_emotes.push_back(emote);

    }
}

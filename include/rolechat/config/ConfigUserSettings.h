#ifndef ROLECHAT_CONFIG_CONFIG_USER_SETTINGS_H
#define ROLECHAT_CONFIG_CONFIG_USER_SETTINGS_H
#include <string>
#include <unordered_map>
namespace rolechat::config {
class ConfigUserSettings
{
public:
    static void save();
    static void load();

    static void setValue(std::string key, bool value) { m_configBooleans[key] = value;}
    static void setValue(std::string key, int value) { m_configIntegers[key] = value;}
    static void setValue(std::string key, float value) { m_configFloats[key] = value;}
    static void setString(std::string key, const std::string& value) { m_configStrings[key] = value;}

    static int intergerValue(const std::string &key, int defaultValue = 0) {
        auto it = m_configIntegers.find(key);
        return it != m_configIntegers.end() ? it->second : defaultValue;
    }

    static bool booleanValue(const std::string &key, bool defaultValue = false) {
        auto it = m_configBooleans.find(key);
        return it != m_configBooleans.end() ? it->second : defaultValue;
    }

    static std::string stringValue(const std::string &key, const std::string &defaultValue = "") {
        auto it = m_configStrings.find(key);
        return it != m_configStrings.end() ? it->second : defaultValue;
    }

    static float floatValue(const std::string &key, float defaultValue = 0.0f) {
        auto it = m_configFloats.find(key);
        return it != m_configFloats.end() ? it->second : defaultValue;
    }

private:
    static std::unordered_map<std::string, std::string> m_configStrings;
    static std::unordered_map<std::string, float> m_configFloats;
    static std::unordered_map<std::string, int> m_configIntegers;
    static std::unordered_map<std::string, bool> m_configBooleans;
};
}
#endif
#include "rolechat/background/JsonBackgroundData.h"
#include "rolechat/filesystem/RCFile.h"
#include "rolechat/filesystem/RCDir.h"
#include "rolechat/util/FileSystem.h"
#include <iostream>
#include <vector>
#include <string>

void printDivider(const std::string& title) {
    std::cout << "\n==== " << title << " ====\n";
}

void printFileStatus(const std::string& path, bool exists) {
    std::cout << "  " << (exists ? "[Exists] " : "[Missing] ") << path << "\n";
}

void test_basic_json_read() {
    printDivider("Basic JSON Read");
    rolechat::background::JsonBackgroundData jsonData;
    jsonData.loadBackground("background.json");
    std::cout << "Loaded background.json\n";
}

void test_package_manager()
{
    printDivider("Package Manager");
    auto packages = rolechat::fs::PackageManager::scanPackages();
    std::cout << "Packages found:\n";
    for (const auto& pkg : packages)
        std::cout << "  " << pkg << "\n";
    auto disabled = rolechat::fs::PackageManager::disabledList();
    std::cout << "Disabled packages:\n";
    for (const auto& pkg : disabled)
        std::cout << "  " << pkg << "\n";
}

void test_rc_directory()
{
    printDivider("RC Directory");
    using namespace rolechat::fs;
    RCDir rcDir("characters");
    auto allDirs = rcDir.findAll();
    std::cout << "Directories named 'characters':\n";
    for (const std::string& path : allDirs) {
        printFileStatus(path, RCDir::exists(path));
    }
    std::cout << "Subfolders within 'characters':\n";
    for (const std::string& path : rcDir.subDirectories()) {
        printFileStatus(path, true);
    }

    std::cout << "Subfolders within base characters:\n";
    for (const std::string& path : RCDir::subDirectories("base/characters")) {
        printFileStatus(path, true);
    }

    std::cout << "Character Animations:\n";
    for (const std::string& path : RCDir("animations/characters").fileList("json", true)) {
        printFileStatus(path, true);
    }

    std::cout << "Build Files:\n";
    for (const std::string& path : RCDir::fileList("build/debug", "", true)) {
        printFileStatus(path, true);
    }

}

void test_rc_file()
{
    printDivider("RC File");
    using namespace rolechat::fs;

    RCFile rcFile("wow.txt");
    std::string filePath = rcFile.findFirst();
    printFileStatus(filePath, rcFile.exists());

    RCFile invalidFile("sldjkfos9u8efhsoie");
    std::string invalidFilePath = invalidFile.findFirst();
    printFileStatus(invalidFilePath, invalidFile.exists());

    std::vector<std::string> extensions = {".txt", ".md"};
    RCFile allFiles("test", extensions);
    std::cout << "Files named 'test' with extensions '.txt' and '.md':\n";
    for (const std::string& path : allFiles.findAll()) {
        printFileStatus(path, RCFile::exists(path));
    }
}

int main() {
    test_package_manager();
    test_rc_file();
    test_rc_directory();
    return 0;
}

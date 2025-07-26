#include "rolechat/background/JsonBackgroundData.h"
#include "rolechat/filesystem/RCFile.h"
#include "rolechat/util/FileSystem.h"
#include <iostream>

void test_basic_json_read() {
    rolechat::background::JsonBackgroundData jsonData;
    jsonData.loadBackground("background.json");
}

void test_package_manager()
{
    rolechat::fs::PackageManager::scanPackages();
}

void test_rc_file()
{
    using namespace rolechat::fs;

    RCFile rcFile("wow.txt");
    std::string filePath = rcFile.findFirst(); 
    std::cout << "[RCFile] Searching for valid file \"wow.txt\", path returned: " << filePath << "\n";

    if(rcFile.exists())
        std::cout << "[RCFile] File exists: " << filePath << "\n";
    else
        std::cout << "[RCFile] File does not exist: " << filePath << "\n";   

    RCFile invalidFile("sldjkfos9u8efhsoie");
    std::string invalidFilePath = invalidFile.findFirst(); 
    std::cout << "[RCFile] Searching for invalid file \"sldjkfos9u8efhsoie\", path returned: " << invalidFilePath << "\n";

    if(invalidFile.exists())
        std::cout << "[RCFile] File exists: " << invalidFilePath << "\n";
    else
        std::cout << "[RCFile] File does not exist: " << invalidFilePath << "\n";   

    std::vector<std::string> extensions = {".txt", ".md"};
    RCFile allFiles("test", extensions);

    std::cout << "[RCFile] Looking for files with name 'test' with extensions of 'txt' and 'md'"<< "\n";
    for (const std::string& pathList : allFiles.findAll())
    {
        std::cout << "  [RCFile] Found path for file 'test' with extensions of 'txt' and 'md': " << pathList << "\n";
        if(RCFile::exists(pathList))
            std::cout << "  [RCFile] File exists: " << pathList << "\n";
        else
            std::cout << "  [RCFile] File does not exist: " << pathList << "\n";
    }

}

int main() {
    test_package_manager();
    test_rc_file();
    return 0;
}

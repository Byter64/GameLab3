#pragma once
#include <string>
#include <map>
#include <filesystem>

namespace Defines
{
    //Path to game defines, including file extension
    extern const std::filesystem::path path;
    extern std::map<std::string, std::string> defines;

    void InitializeDefines();

    int Int(std::string key);
    std::string String(std::string key);
    bool Bool(std::string key);
    float Float(std::string key);
}

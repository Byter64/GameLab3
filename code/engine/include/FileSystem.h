#pragma once
#include <filesystem>
#include <vector>
#include <string>

namespace Engine::Files
{
    extern const std::filesystem::path ASSETS;
    extern const std::filesystem::path LOGS;
    extern const std::filesystem::path LOG;
    std::vector<std::string> ParseFile(std::filesystem::path path);
}
// Engine

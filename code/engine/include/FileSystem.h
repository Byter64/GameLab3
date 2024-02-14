#pragma once
#include <filesystem>
#include <vector>
#include <string>

namespace Engine::Files
{
    extern const std::filesystem::path ASSETS;
    std::vector<std::string> ParseFile(std::filesystem::path path);
}
// Engine

#pragma once
#include "ECSSystem.h"
#include <vector>
#include <filesystem>

class Dungeon
{
public:
    inline static const float dungeonRotation = -20;

    Engine::Entity gameObject;
    std::vector<std::vector<bool>> wallMap;

    Dungeon(std::filesystem::path pathToImage, Engine::Entity wallPrefab);
};

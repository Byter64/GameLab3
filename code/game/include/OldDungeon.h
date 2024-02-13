#pragma once
#include "ECSSystem.h"
#include <vector>
#include <filesystem>

class OldDungeon
{
public:
    inline static const float dungeonRotation = -20;

    Engine::Entity entity;
    std::vector<std::vector<bool>> wallMap;

    OldDungeon(std::filesystem::path pathToImage, Engine::Entity wallPrefab);
};

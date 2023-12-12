#pragma once
#include "GameObject.h"
#include <vector>
#include <filesystem>

class Dungeon
{
public:
    inline static const float dungeonRotation = 0;

    GameObject* gameObject;
    std::vector<std::vector<bool>> wallMap;

    Dungeon(std::filesystem::path pathToImage, GameObject* wallPrefab);
};

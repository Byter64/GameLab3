#pragma once
#include <filesystem>
#include <utility>
#include <map>
#include <list>
#include "ECS/Entity.h"

namespace Engine
{

    struct Dungeon
    {
        std::filesystem::path pathToDungeons;
        std::string fileName;
        std::map<std::pair<int, int>, std::list<std::string>> enemies;
        int activeDungeon;
        std::map<std::pair<int, int>, Entity> activeEnemies;
    };

} // Engine

#pragma once
#include <filesystem>
#include <utility>
#include <map>
#include <list>
#include "ECS/Entity.h"
#include "ECS/Components/EnemyBehaviour.h"

struct Dungeon
{
    std::filesystem::path pathToDungeons;
    std::string fileName;
    std::map<std::pair<int, int>, std::list<Engine::EnemyBehaviour::Behaviour>> enemies;
    std::map<std::pair<int, int>, Engine::Entity> activeEnemies;
    int activeDungeon = 0;

    Dungeon() = default;
    Dungeon(std::filesystem::path pathToDungeons, std::string fileName);
};

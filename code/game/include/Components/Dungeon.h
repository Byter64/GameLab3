#pragma once
#include <filesystem>
#include <utility>
#include <map>
#include <list>
#include "ECS/Entity.h"
#include "EnemyBehaviour.h"
#include <queue>
#include <functional>
#include <chrono>

struct Dungeon
{
    std::filesystem::path pathToDungeons;
    std::string fileName;

    //A map, that maps a position within the dungeon, to a time and an enemy. this enemy is supposed to be spawned at the given time and position
    std::map<std::pair<int, int>, std::priority_queue<std::pair<float, EnemyBehaviour::Type>, std::vector<std::pair<float, EnemyBehaviour::Type>>, std::greater<>>> enemies;
    std::map<std::pair<int, int>, Engine::Entity> activeEnemies;
    int activeDungeon = 0;
    float creationTime;
    bool areAllEnemiesDefeated = false;
    Dungeon();
    Dungeon(std::filesystem::path pathToDungeons, std::string fileName);
};

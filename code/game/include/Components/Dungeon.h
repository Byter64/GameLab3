#pragma once
#include <filesystem>
#include <utility>
#include <list>
#include <list>
#include "ECS/Entity.h"
#include "EnemyBehaviour.h"
#include <queue>
#include <functional>
#include <chrono>
#include "Helpers/EnemySpawnerData.h"

struct Dungeon
{
    std::filesystem::path pathToDungeons;
    std::string fileName;

    //A map, that maps a position within the dungeon, to a time and an enemy. this enemy is supposed to be spawned at the given time and position
    std::list<EnemySpawnerData> spawnerData;
    std::list<Engine::Entity> activeEnemies;
    int activeDungeonIndex = 0;
    float referenceSpawnTime = 0;

    std::vector<std::vector<bool>> wallMap;
    std::pair<int, int> size;
    glm::vec2 originOffset;
    Dungeon();
    Dungeon(std::filesystem::path pathToDungeons, std::string fileName);
};

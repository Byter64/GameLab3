#pragma once
#include "Components/Enemies/EnemyBehaviour.h"
#include <utility>

struct EnemySpawnerData
{
    EnemyBehaviour::Type type;
    std::pair<int, int> spawnPosition;
    float spawnTime; //relative to last block statement
    bool waitUntilEveryoneDied; //if this is true, this enemy will only be spawned after all previous enemies are defeated
};

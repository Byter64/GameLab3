#include "Components/Dungeon.h"

Dungeon::Dungeon(std::filesystem::path pathToDungeons, std::string fileName) : pathToDungeons(pathToDungeons), fileName(fileName),
spawnerData(), activeEnemies(), activeDungeon(0)
{
    referenceSpawnTime = Engine::Systems::timeManager->GetTimeSinceStartup();
}

Dungeon::Dungeon()
{
    referenceSpawnTime = Engine::Systems::timeManager->GetTimeSinceStartup();
}

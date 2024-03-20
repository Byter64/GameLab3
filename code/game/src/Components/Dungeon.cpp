#include "Components/Dungeon.h"

Dungeon::Dungeon(std::filesystem::path pathToDungeons, std::string fileName) : pathToDungeons(pathToDungeons), fileName(fileName),
enemies(std::map<std::pair<int, int>, std::priority_queue<std::pair<float, EnemyBehaviour::Behaviour>, std::vector<std::pair<float, EnemyBehaviour::Behaviour>>, std::greater<>>>()),
activeEnemies(std::map<std::pair<int, int>, Engine::Entity>()), activeDungeon(0)
{
    creationTime = Engine::Systems::timeManager->GetTimeSinceStartup();
}

Dungeon::Dungeon()
{
    creationTime = Engine::Systems::timeManager->GetTimeSinceStartup();
}

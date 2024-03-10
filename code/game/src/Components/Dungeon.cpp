#include "Components/Dungeon.h"

Dungeon::Dungeon(std::filesystem::path pathToDungeons, std::string fileName) : pathToDungeons(pathToDungeons), fileName(fileName),
enemies(std::map<std::pair<int, int>, std::list<EnemyBehaviour::Behaviour>>()), activeEnemies(std::map<std::pair<int, int>, Engine::Entity>()), activeDungeon(0)
{

}
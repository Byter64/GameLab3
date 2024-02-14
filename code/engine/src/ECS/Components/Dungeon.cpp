#include "ECS/Components/Dungeon.h"


namespace Engine
{
    Engine::Dungeon::Dungeon(std::filesystem::path pathToDungeons, std::string fileName) : pathToDungeons(pathToDungeons), fileName(fileName),
    enemies(std::map<std::pair<int, int>, std::list<EnemyBehaviour::Behaviour>>()), activeEnemies(std::map<std::pair<int, int>, Entity>()), activeDungeon(0)
    {

    }

} // Engine
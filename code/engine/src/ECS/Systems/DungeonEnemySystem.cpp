#include "ECS/Systems/DungeonEnemySystem.h"
#include "Engine.h"
#include "ECSExtension.h"
extern std::shared_ptr<Engine::DungeonSystem> dungeonSystem; //Never change this name, as Systems depend on this symbol
namespace Engine
{
    void DungeonEnemySystem::EntityAdded(Entity entity)
    {

    }

    void DungeonEnemySystem::EntityRemoved(Entity entity)
    {
        for(Entity dungeonEntity : dungeonSystem->entities)
        {
            Dungeon& dungeon = ecsSystem->GetComponent<Dungeon>(dungeonEntity);

            std::pair<int, int> pos;
            bool isActive = false;
            for(auto& pair : dungeon.activeEnemies)
            {
                if(entity == pair.second)
                {
                    isActive = true;
                    pos = pair.first;
                    break;
                }
            }

            if(isActive)
            {
                dungeon.activeEnemies.erase(pos);
            }
        }
    }
} // Engine
#include "Systems/DungeonEnemySystem.h"
#include "Engine.h"
#include "ECSExtension.h"

extern std::shared_ptr<DungeonSystem> dungeonSystem; //Never change this name, as Systems depend on this symbol

void DungeonEnemySystem::EntityAdded(Engine::Entity entity)
{

}

void DungeonEnemySystem::EntityRemoved(Engine::Entity entity)
{
    for(Engine::Entity dungeonEntity : dungeonSystem->entities)
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
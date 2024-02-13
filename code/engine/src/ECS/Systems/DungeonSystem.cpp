#include "ECS/Systems/DungeonSystem.h"
#include <stdexcept>
#include "Engine.h"
namespace Engine
{
    void DungeonSystem::EntityAdded(Entity entity)
    {
        if(entities.size() != 0)
        {
            std::string message{ "There is already an entity with a Dungeon component"};
            throw std::runtime_error(message);
        }
    }

    void DungeonSystem::EntityRemoved(Entity entity)
    {

    }

    void DungeonSystem::Update()
    {
        for(Entity entity : entities)
        {
            Dungeon& dungeon = ecsSystem->GetComponent<Dungeon>(entity);

            //Spawn enemies
            for(auto& pair : dungeon.enemies)
            {
                if(dungeon.activeEnemies.count(pair.first) == 0)
                {
                    if(dungeon.enemies.empty())
                    {
                        //Go to next dungeon
                        std::cout << "All enemies are defeated, you win" << std::endl;
                    }
                    else if(dungeon.enemies.count(pair.first))
                    {
                        Entity hubertus = SpawnEnemy(glm::vec3(pair.first.first, pair.first.second, 0),
                                                     EnemyBehaviour::stringToBehaviour.at(dungeon.enemies[pair.first].front()));
                        dungeon.enemies[pair.first].pop_front();
                        if(dungeon.enemies[pair.first].empty())
                            dungeon.enemies.erase(pair.first);
                        dungeon.activeEnemies[pair.first] = hubertus;
                    }
                }
            }
        }
    }
} // Engine
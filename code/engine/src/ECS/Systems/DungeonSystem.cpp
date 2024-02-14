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
                        std::cout << "All enemies are defeated, you win" << std::endl;
                        ReadInNewDungeon(entity);
                    }
                    else if(dungeon.enemies.count(pair.first))
                    {
                        Entity hubertus = SpawnEnemy(glm::vec3(pair.first.first, pair.first.second, 0),
                                                     dungeon.enemies[pair.first].front());
                        dungeon.enemies[pair.first].pop_front();
                        if(dungeon.enemies[pair.first].empty())
                            dungeon.enemies.erase(pair.first);
                        dungeon.activeEnemies[pair.first] = hubertus;
                    }
                }
            }
        }


    }

    void DungeonSystem::ReadInNewDungeon(Entity entity)
    {
        Dungeon& dungeon = ecsSystem->GetComponent<Dungeon>(entity);

        dungeon.activeDungeon++;

        std::vector<std::string> file;
        bool noDungeon = false;
        try
        {
            file = Engine::Files::ParseFile(dungeon.pathToDungeons / (dungeon.fileName + std::to_string(dungeon.activeDungeon)));
        }
        catch (std::runtime_error& bla)
        {
            noDungeon = true;
        }
        if(noDungeon)
        {
            std::cout << "Game is over" << std::endl;
            exit(-1);
        }

        for(int i = 0; i < file.size(); i += 3)
        {
            EnemyBehaviour::Behaviour behaviour = EnemyBehaviour::stringToBehaviour.at(file[i]);
            std::pair<int, int> pos = {std::stoi(file[i + 1]), std::stoi(file[i + 2])};

            dungeon.enemies[pos].push_back(behaviour);
        }
    }
} // Engine
#include "ECS/Systems/DungeonSystem.h"
#include <stdexcept>
#include "Engine.h"
#include "../tinygltf/stb_image.h"
#define STBI_FAILURE_USERMSG

namespace Engine
{
    void DungeonSystem::EntityAdded(Entity entity)
    {
        if(entities.size() != 0)
        {
            std::string message{ "There is already an entity with a Dungeon component"};
            throw std::runtime_error(message);
        }

        ReadInDungeonMap(entity);
        ReadInEnemies(entity);
    }

    void DungeonSystem::EntityRemoved(Entity entity)
    {

    }

    void DungeonSystem::Update()
    {
        for(Entity entity : entities)
        {
            Dungeon &dungeon = ecsSystem->GetComponent<Dungeon>(entity);

            if(dungeon.enemies.empty() && dungeon.activeEnemies.empty())
            {
                std::cout << "All enemies are defeated, you win" << std::endl;
                dungeon.activeDungeon++;
                try
                {
                    ReadInDungeonMap(entity);
                    ReadInEnemies(entity);
                }
                catch (std::runtime_error& e)
                {
                    throw std::runtime_error("finished game");
                }
            }

            //Spawn enemies
            for (auto &pair: dungeon.enemies)
            {
                if (dungeon.activeEnemies.count(pair.first) == 0)
                {
                    if (dungeon.enemies.count(pair.first))
                    {
                        Entity hubertus = SpawnEnemy(pair.first, dungeon.enemies[pair.first].front());
                        dungeon.enemies[pair.first].pop_front();
                        dungeon.activeEnemies[pair.first] = hubertus;
                    }
                }
            }

            for (auto iter = dungeon.enemies.begin(); iter != dungeon.enemies.end();)
            {
                if (iter->second.empty())
                    iter = dungeon.enemies.erase(iter);
                else
                    ++iter;
            }
        }
    }

    void DungeonSystem::ReadInEnemies(Entity entity)
    {
        Dungeon& dungeon = ecsSystem->GetComponent<Dungeon>(entity);

        std::vector<std::string> file;
        bool noDungeon = false;
        try
        {
            file = Engine::Files::ParseFile(dungeon.pathToDungeons / (dungeon.fileName + std::to_string(dungeon.activeDungeon) + ".txt"));
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

    void DungeonSystem::ReadInDungeonMap(Entity entity)
    {
        Dungeon& dungeon = ecsSystem->GetComponent<Dungeon>(entity);
        RemoveEntityWithChildren(entity, false);

        int width, height, channels;
        std::string path = (dungeon.pathToDungeons / (dungeon.fileName + std::to_string(dungeon.activeDungeon) + ".png")).string();
        unsigned char* image = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
        if(image == nullptr)
        {
            std::string message{"ERROR while loading the image: "};
            message += path;
            message += "\n";
            message += stbi_failure_reason();
            std::cout << message << "\n";
            throw std::runtime_error(message);
        }

        wallMap.clear();
        for(int x = 0; x < width; x++)
        {
            wallMap.push_back(std::vector<bool>());
            for(int y = 0; y < height; y++)
            {
                unsigned char alpha;
                int pos = (((height - y - 1) * width) + x) * 4;
                alpha = *(image + pos + 3);

                bool isWall = alpha >= 128;
                wallMap[x].push_back(isWall);
                if(!isWall) continue;

                std::string name = "Wall(";
                name += std::to_string(x);
                name += "|";
                name += std::to_string(y);
                name += ")";

                Entity wall = SpawnWall(glm::vec3(x - width / 2, y - height / 2,0));

                ecsSystem->GetComponent<Engine::Name>(wall) = name;
                ecsSystem->GetComponent<Engine::Transform>(wall).SetParent(&ecsSystem->GetComponent<Engine::Transform>(entity));
            }
        }
    }
} // Engine
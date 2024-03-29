#include "Systems/DungeonSystem.h"
#include <stdexcept>
#include "Engine.h"
#include "../../../extern/tinygltf/stb_image.h"
#define STBI_FAILURE_USERMSG
#include "ECSExtension.h"

void DungeonSystem::EntityAdded(Engine::Entity entity)
{
    if(entities.size() != 0)
    {
        std::string message{ "There is already an entity with a Dungeon component"};
        throw std::runtime_error(message);
    }

    ReadInDungeonMap(entity);
    ReadInEnemies(entity);
}

void DungeonSystem::EntityRemoved(Engine::Entity entity)
{

}

void DungeonSystem::Update()
{
    for(Engine::Entity entity : entities)
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
                exit(-1);
            }
        }

        //Spawn enemies
        for (auto &pair: dungeon.enemies)
        {
            if (dungeon.activeEnemies.count(pair.first) == 0)
            {
                if (dungeon.enemies.count(pair.first))
                {
                    Engine::Entity enemy;
                    switch(dungeon.enemies[pair.first].front())
                    {
                        case EnemyBehaviour::Hubertus:
                            enemy = ECSHelper::SpawnHubertus(pair.first);
                            break;

                        case EnemyBehaviour::KindredSpirit:
                        {
                            auto enemyPair = ECSHelper::SpawnKindredSpirit(pair.first);
                            enemy = enemyPair.first;
                            std::pair<int, int> secondPos = {pair.first.first * -1, pair.first.second * -1};
                            dungeon.activeEnemies[secondPos] = enemyPair.second;
                            break;
                        }
                        case EnemyBehaviour::Assi:
                            break;
                        case EnemyBehaviour::Cuball:
                            break;
                    }
                    dungeon.enemies[pair.first].pop_front();
                    dungeon.activeEnemies[pair.first] = enemy;
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

void DungeonSystem::ReadInEnemies(Engine::Entity entity)
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

void DungeonSystem::ReadInDungeonMap(Engine::Entity entity)
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

            Engine::Entity wall = ECSHelper::SpawnWall(glm::vec3(x - width / 2, y - height / 2,0));

            ecsSystem->GetComponent<Engine::Name>(wall) = name;
            ecsSystem->GetComponent<Engine::Transform>(wall).SetParent(&ecsSystem->GetComponent<Engine::Transform>(entity));
        }
    }
}
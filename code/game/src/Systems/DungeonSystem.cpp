#include "Systems/DungeonSystem.h"
#include <stdexcept>
#include "Engine.h"
#include "../../../extern/tinygltf/stb_image.h"
#define STBI_FAILURE_USERMSG
#include "ECSExtension.h"
#include <cstring>
#include <algorithm>
#include "CollisionLayer.h"

extern std::shared_ptr<EnemyBehaviourSystem> enemyBehaviourSystem;
extern std::pair<Engine::Entity, Engine::Entity> players;

void DungeonSystem::EntityAdded(Engine::Entity entity)
{

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
            dungeon.areAllEnemiesDefeated = true;
            continue;
        }
        //Spawn enemies
        for (auto &pair: dungeon.enemies)
        {
            if (dungeon.activeEnemies.count(pair.first) == 0 &&
            pair.second.top().first < (Engine::Systems::timeManager->GetTimeSinceStartup() - dungeon.creationTime))
            {
                std::vector<Engine::Entity> enemies;
                switch (pair.second.top().second)
                {
                    case EnemyBehaviour::Hubertus:
                        enemies.push_back(ECSHelper::SpawnHubertus(pair.first));
                        break;

                    case EnemyBehaviour::KindredSpirit:
                    {
                        auto enemyPair = ECSHelper::SpawnKindredSpirit(pair.first);
                        enemies.push_back(enemyPair.second);
                        enemies.push_back(enemyPair.first);
                        std::pair<int, int> secondPos = {pair.first.first * -1, pair.first.second * -1};
                        dungeon.activeEnemies[secondPos] = enemyPair.second;
                        break;
                    }
                    case EnemyBehaviour::Assi:
                        enemies.push_back(ECSHelper::SpawnAssi(pair.first));
                        break;
                    case EnemyBehaviour::Cuball:
                        enemies.push_back(ECSHelper::SpawnCuball(pair.first));
                        break;
                }
                pair.second.pop();
                dungeon.activeEnemies[pair.first] = enemies.back();

                for(Engine::Entity spawnedEntity : enemies)
                {
                    if(ecsSystem->GetComponent<EnemyBehaviour>(spawnedEntity).behaviour == EnemyBehaviour::Cuball)
                    {
                        Engine::Systems::animationSystem->PlayAnimation(spawnedEntity, "Cuball_Spawning");
                    }
                    else
                    {
                        Engine::Entity elevator = ECSHelper::SpawnElevator(ecsSystem->GetComponent<Engine::Transform>(spawnedEntity).GetTranslation(),spawnedEntity);
                        Engine::Systems::animationSystem->PlayAnimation(elevator, "Elevator_Spawning");
                    }
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
    static const std::string WAIT_KEYWORD = "wait";
    static const std::string SINGLE_KEYWORD = "singleplayer";
    static const std::string MULTI_KEYWORD = "twoplayers";
    static const std::string MIRROR_KEYWORD = "mirror";

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

    enum
    {
        Nothing,
        ExpectingTime,
        ExpectingXPos,
        ExpectingYPos
    } state = Nothing;

    float time = 0;
    EnemyBehaviour::Type behaviour;
    std::pair<int, int> pos;
    float timeDelta;
    for (int i = 0; i < file.size(); i++)
    {
        std::string& symbol = file[i];
        std::transform(symbol.begin(), symbol.end(), symbol.begin(), tolower);

        switch (state)
        {
            case Nothing:
                if(symbol == WAIT_KEYWORD)
                    state = ExpectingTime;
                else if(symbol == SINGLE_KEYWORD && players.second == Engine::Entity::INVALID_ENTITY_ID)
                {
                    Engine::Transform& transform1 = ecsSystem->GetComponent<Engine::Transform>(players.first);
                    std::pair<int, int> pos1 = {std::stoi(file[i + 1]), std::stoi(file[i + 2])};
                    transform1.SetTranslation(glm::vec3(EnemyBehaviourSystem::ToGlobal(pos1), 0.0f));
                }
                else if(symbol == MULTI_KEYWORD && players.second != Engine::Entity::INVALID_ENTITY_ID)
                {
                    Engine::Transform& transform1 = ecsSystem->GetComponent<Engine::Transform>(players.first);
                    std::pair<int, int> pos1 = {std::stoi(file[i + 1]), std::stoi(file[i + 2])};
                    transform1.SetTranslation(glm::vec3(EnemyBehaviourSystem::ToGlobal(pos1), 0.0f));

                    Engine::Transform& transform2 = ecsSystem->GetComponent<Engine::Transform>(players.second);
                    std::pair<int, int> pos2 = {std::stoi(file[i + 3]), std::stoi(file[i + 4])};
                    transform2.SetTranslation(glm::vec3(EnemyBehaviourSystem::ToGlobal(pos2), 0.0f));
                }
                else if(symbol == MIRROR_KEYWORD)
                {
                    std::pair<int, int> mirrors{std::stoi(file[i + 1]), std::stoi(file[i + 2])};
                    CuballExtra::mirrorDirection = mirrors;
                }
                else if(EnemyBehaviour::stringToBehaviour.count(symbol))
                {
                    behaviour = EnemyBehaviour::stringToBehaviour.at(symbol);
                    state = ExpectingXPos;
                }
                break;
            case ExpectingTime:
                try { timeDelta = std::stof(symbol); }
                catch (std::invalid_argument exception)
                {
                    std::cout << "a time value was expected but none was given. Problematic symbol: " << file[i] << std::endl;
                    exit(-1);
                }
                time += timeDelta;
                state = Nothing;
                break;

            case ExpectingXPos:
                try { pos.first = std::stoi(symbol); }
                catch (std::invalid_argument exception)
                {
                    std::cout << "a value for x position was expected but none was given. Problematic symbol: " << file[i] << std::endl;
                    exit(-1);
                }
                state = ExpectingYPos;
                break;
            case ExpectingYPos:
                try { pos.second = std::stoi(symbol); }
                catch (std::invalid_argument exception)
                {
                    std::cout << "a value for y position was expected but none was given. Problematic symbol: " << file[i] << std::endl;
                    exit(-1);
                }
                dungeon.enemies[pos].push({time, behaviour});
                state = Nothing;
                break;
        }
    }
}

void DungeonSystem::ReadInDungeonMap(Engine::Entity entity)
{
    Dungeon& dungeon = ecsSystem->GetComponent<Dungeon>(entity);
    dungeon.creationTime = Engine::Systems::timeManager->GetTimeSinceStartup();
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
    enemyBehaviourSystem->ChangeWallMap(wallMap);

    Engine::TilemapCollider& collider = ecsSystem->AddComponent<Engine::TilemapCollider>(entity);
    collider.map = wallMap;
    collider.layer = (unsigned char)CollisionLayer::Dungeon;
    collider.position = {wallMap.size() / 2, wallMap[0].size() / 2, 0};
    collider.position *= -1;

}

bool DungeonSystem::TryLoadTestDungeonAndEnemies(Engine::Entity entity)
{
    Dungeon& dungeon = ecsSystem->GetComponent<Dungeon>(entity);
    dungeon.creationTime = Engine::Systems::timeManager->GetTimeSinceStartup();
    RemoveEntityWithChildren(entity, false);

    int width, height, channels;
    std::string path = (dungeon.pathToDungeons / (dungeon.fileName + "x" + ".png")).string();
    unsigned char* image = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
    if(image == nullptr)
    {
        return false;
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
    enemyBehaviourSystem->ChangeWallMap(wallMap);

    Engine::TilemapCollider& collider = ecsSystem->AddComponent<Engine::TilemapCollider>(entity);
    collider.map = wallMap;
    collider.layer = (unsigned char)CollisionLayer::Dungeon;
    collider.position = {wallMap.size() / 2, wallMap[0].size() / 2, 0};
    collider.position *= -1;

    static const std::string WAIT_KEYWORD = "wait";
    static const std::string SINGLE_KEYWORD = "singleplayer";
    static const std::string MULTI_KEYWORD = "twoplayers";
    static const std::string MIRROR_KEYWORD = "mirror";

    std::vector<std::string> file;
    bool noDungeon = false;
    try
    {
        file = Engine::Files::ParseFile(dungeon.pathToDungeons / (dungeon.fileName + "x" + ".txt"));
    }
    catch (std::runtime_error& bla)
    {
        noDungeon = true;
    }
    if(noDungeon)
    {
        return false;
    }

    enum
    {
        Nothing,
        ExpectingTime,
        ExpectingXPos,
        ExpectingYPos
    } state = Nothing;

    float time = 0;
    EnemyBehaviour::Type behaviour;
    std::pair<int, int> pos;
    float timeDelta;
    for (int i = 0; i < file.size(); i++)
    {
        std::string& symbol = file[i];
        std::transform(symbol.begin(), symbol.end(), symbol.begin(), tolower);

        switch (state)
        {
            case Nothing:
                if(symbol == WAIT_KEYWORD)
                    state = ExpectingTime;
                else if(symbol == SINGLE_KEYWORD && players.second == Engine::Entity::INVALID_ENTITY_ID)
                {
                    Engine::Transform& transform1 = ecsSystem->GetComponent<Engine::Transform>(players.first);
                    std::pair<int, int> pos1 = {std::stoi(file[i + 1]), std::stoi(file[i + 2])};
                    transform1.SetTranslation(glm::vec3(EnemyBehaviourSystem::ToGlobal(pos1), 0.0f));
                }
                else if(symbol == MULTI_KEYWORD && players.second != Engine::Entity::INVALID_ENTITY_ID)
                {
                    Engine::Transform& transform1 = ecsSystem->GetComponent<Engine::Transform>(players.first);
                    std::pair<int, int> pos1 = {std::stoi(file[i + 1]), std::stoi(file[i + 2])};
                    transform1.SetTranslation(glm::vec3(EnemyBehaviourSystem::ToGlobal(pos1), 0.0f));

                    Engine::Transform& transform2 = ecsSystem->GetComponent<Engine::Transform>(players.second);
                    std::pair<int, int> pos2 = {std::stoi(file[i + 3]), std::stoi(file[i + 4])};
                    transform2.SetTranslation(glm::vec3(EnemyBehaviourSystem::ToGlobal(pos2), 0.0f));
                }
                else if(symbol == MIRROR_KEYWORD)
                {
                    std::pair<int, int> mirrors{std::stoi(file[i + 1]), std::stoi(file[i + 2])};
                    CuballExtra::mirrorDirection = mirrors;
                }
                else if(EnemyBehaviour::stringToBehaviour.count(symbol))
                {
                    behaviour = EnemyBehaviour::stringToBehaviour.at(symbol);
                    state = ExpectingXPos;
                }
                break;
            case ExpectingTime:
                try { timeDelta = std::stof(symbol); }
                catch (std::invalid_argument exception)
                {
                    std::cout << "a time value was expected but none was given. Problematic symbol: " << file[i] << std::endl;
                    exit(-1);
                }
                time += timeDelta;
                state = Nothing;
                break;

            case ExpectingXPos:
                try { pos.first = std::stoi(symbol); }
                catch (std::invalid_argument exception)
                {
                    std::cout << "a value for x position was expected but none was given. Problematic symbol: " << file[i] << std::endl;
                    exit(-1);
                }
                state = ExpectingYPos;
                break;
            case ExpectingYPos:
                try { pos.second = std::stoi(symbol); }
                catch (std::invalid_argument exception)
                {
                    std::cout << "a value for y position was expected but none was given. Problematic symbol: " << file[i] << std::endl;
                    exit(-1);
                }
                dungeon.enemies[pos].push({time, behaviour});
                state = Nothing;
                break;
        }
    }

    dungeon.activeDungeon--;
    return true;
}

void DungeonSystem::UpdateDungeon(Engine::Entity entity)
{
    Dungeon& dungeon = ecsSystem->GetComponent<Dungeon>(entity);
    dungeon.activeDungeon++;
    dungeon.areAllEnemiesDefeated = false;
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

void DungeonSystem::InitializeDungeons()
{
    if(entities.size() != 1)
    {
        std::string message{ "There are too many entities with a Dungeon component"};
        throw std::runtime_error(message);
    }

    for (Engine::Entity entity :entities)
    {
        if(!TryLoadTestDungeonAndEnemies(entity))
        {
            ReadInDungeonMap(entity);
            ReadInEnemies(entity);
        }
    }
}

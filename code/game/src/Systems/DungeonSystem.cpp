#include "Systems/DungeonSystem.h"
#include <stdexcept>
#include "Engine.h"
#include "../../../extern/tinygltf/stb_image.h"
#define STBI_FAILURE_USERMSG
#include "ECSExtension.h"
#include <cstring>
#include <algorithm>
#include "CollisionLayer.h"
#include <stdarg.h>

extern std::shared_ptr<EnemyBehaviourSystem> enemyBehaviourSystem;
extern std::pair<Engine::Entity, Engine::Entity> players;

void DungeonSystem::EntityAdded(Engine::Entity entity)
{

}

void DungeonSystem::EntityRemoved(Engine::Entity entity)
{

}

void DungeonSystem::Initialize()
{
    entity = ecsSystem->CreateEntity();
    ecsSystem->AddComponent<Engine::Name>(entity, "Dungeon");
    ecsSystem->AddComponent(entity, Engine::Transform());
    ecsSystem->AddComponent<Dungeon>(entity, Dungeon(Engine::Files::ASSETS / "Dungeons", "Dungeon_"));

    Dungeon &dungeon = ecsSystem->GetComponent<Dungeon>(entity);
    std::filesystem::path enemyFile = dungeon.pathToDungeons / (dungeon.fileName + "x" + ".txt");
    std::filesystem::path dungeonFile = dungeon.pathToDungeons / (dungeon.fileName + "x" + ".png");

    if (!(std::filesystem::exists(enemyFile) && std::filesystem::exists(dungeonFile)))
    {
        enemyFile = dungeon.pathToDungeons / (dungeon.fileName + std::to_string(dungeon.activeDungeon) + ".txt");
        dungeonFile = dungeon.pathToDungeons / (dungeon.fileName + std::to_string(dungeon.activeDungeon) + ".png");
    }

    ReadInDungeonMap(dungeon, dungeonFile.string());
    ReadInEnemies(dungeon, enemyFile.string());
}

void DungeonSystem::Update()
{
    Dungeon &dungeon = ecsSystem->GetComponent<Dungeon>(entity);

    if(dungeon.spawnerData.empty()) return;

    EnemySpawnerData& frontEnemy = dungeon.spawnerData.front();

    if(frontEnemy.spawnTime > Engine::Systems::timeManager->GetTimeSinceStartup() - dungeon.referenceSpawnTime) return;
    if(frontEnemy.waitUntilEveryoneDied && !dungeon.activeEnemies.empty()) return;

    if(frontEnemy.waitUntilEveryoneDied)
        dungeon.referenceSpawnTime = Engine::Systems::timeManager->GetTimeSinceStartup();

    std::vector<Engine::Entity> enemies;
    switch (frontEnemy.type)
    {
        case EnemyBehaviour::Hubertus:
            enemies.push_back(ECSHelper::SpawnHubertus(frontEnemy.spawnPosition));
            break;
        case EnemyBehaviour::KindredSpirit: {
            auto enemyPair = ECSHelper::SpawnKindredSpirit(frontEnemy.spawnPosition);
            enemies.push_back(enemyPair.second);
            enemies.push_back(enemyPair.first);
            break; }
        case EnemyBehaviour::Assi:
            enemies.push_back(ECSHelper::SpawnAssi(frontEnemy.spawnPosition));
            break;
        case EnemyBehaviour::Cuball:
            enemies.push_back(ECSHelper::SpawnCuball(frontEnemy.spawnPosition));
            break;
        case EnemyBehaviour::Duke:
            enemies.push_back(ECSHelper::SpawnDuke(frontEnemy.spawnPosition));
            break;
    }

    dungeon.spawnerData.pop_front();
    for(Engine::Entity enemy : enemies) dungeon.activeEnemies.push_back(enemy);

    for (Engine::Entity spawnedEntity: enemies)
    {
        ecsSystem->GetComponent<Engine::BoxCollider>(spawnedEntity).layer = (int) CollisionLayer::Ignore;
        if (ecsSystem->GetComponent<EnemyBehaviour>(spawnedEntity).behaviour == EnemyBehaviour::Cuball)
        {
            Engine::Systems::animationSystem->PlayAnimation(spawnedEntity, "Cuball_Spawning");
        } else
        {
            Engine::Entity elevator = ECSHelper::SpawnElevator(ecsSystem->GetComponent<Engine::Transform>(spawnedEntity).GetTranslation(), spawnedEntity);
            Engine::Systems::animationSystem->PlayAnimation(elevator, "Elevator_Spawning");
        }
    }
}

void DungeonSystem::ReadInEnemies(Dungeon& dungeon, std::string file)
{
    static const std::string WAIT_KEYWORD = "wait";
    static const std::string BLOCK_KEYWORD = "block";
    static const std::string SINGLE_KEYWORD = "singleplayer";
    static const std::string MULTI_KEYWORD = "twoplayers";
    static const std::string MIRROR_KEYWORD = "mirror";
    static const std::string PREPPOS_KEYWORD = "preppos";

    DukeExtra::preparationPositions.clear();

    std::vector<std::string> filecontent;
    bool noDungeon = false;
    try
    {
        filecontent = Engine::Files::ParseFile(file);
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
    bool doesEnemyBlock = false;
    for (int i = 0; i < filecontent.size(); i++)
    {
        std::string& symbol = filecontent[i];
        std::transform(symbol.begin(), symbol.end(), symbol.begin(), tolower);

        switch (state)
        {
            case Nothing:
                if(symbol == WAIT_KEYWORD)
                    state = ExpectingTime;
                else if(symbol == BLOCK_KEYWORD)
                {
                    doesEnemyBlock = true;
                }
                else if(symbol == SINGLE_KEYWORD && players.second == Engine::Entity::INVALID_ENTITY_ID)
                {
                    Engine::Transform& transform1 = ecsSystem->GetComponent<Engine::Transform>(players.first);
                    std::pair<int, int> pos1 = {std::stoi(filecontent[i + 1]), std::stoi(filecontent[i + 2])};
                    transform1.SetTranslation(glm::vec3(EnemyBehaviourSystem::ToGlobal(pos1), 0.0f));
                }
                else if(symbol == MULTI_KEYWORD && players.second != Engine::Entity::INVALID_ENTITY_ID)
                {
                    Engine::Transform& transform1 = ecsSystem->GetComponent<Engine::Transform>(players.first);
                    std::pair<int, int> pos1 = {std::stoi(filecontent[i + 1]), std::stoi(filecontent[i + 2])};
                    transform1.SetTranslation(glm::vec3(EnemyBehaviourSystem::ToGlobal(pos1), 0.0f));

                    Engine::Transform& transform2 = ecsSystem->GetComponent<Engine::Transform>(players.second);
                    std::pair<int, int> pos2 = {std::stoi(filecontent[i + 3]), std::stoi(filecontent[i + 4])};
                    transform2.SetTranslation(glm::vec3(EnemyBehaviourSystem::ToGlobal(pos2), 0.0f));
                }
                else if(symbol == MIRROR_KEYWORD)
                {
                    std::pair<int, int> mirrors{std::stoi(filecontent[i + 1]), std::stoi(filecontent[i + 2])};
                    CuballExtra::mirrorDirection = mirrors;
                }
                else if(symbol == PREPPOS_KEYWORD)
                {
                    std::pair<int, int> prepPos{std::stoi(filecontent[i + 1]), std::stoi(filecontent[i + 2])};
                    DukeExtra::preparationPositions.push_back(prepPos);
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
                    std::cout << "a time value was expected but none was given. Problematic symbol: " << filecontent[i] << std::endl;
                    exit(-1);
                }
                time += timeDelta;
                state = Nothing;
                break;

            case ExpectingXPos:
                try { pos.first = std::stoi(symbol); }
                catch (std::invalid_argument exception)
                {
                    std::cout << "a value for x position was expected but none was given. Problematic symbol: " << filecontent[i] << std::endl;
                    exit(-1);
                }
                state = ExpectingYPos;
                break;
            case ExpectingYPos:
                try { pos.second = std::stoi(symbol); }
                catch (std::invalid_argument exception)
                {
                    std::cout << "a value for y position was expected but none was given. Problematic symbol: " << filecontent[i] << std::endl;
                    exit(-1);
                }
                dungeon.spawnerData.push_back({behaviour, pos, time, doesEnemyBlock});
                if(doesEnemyBlock)
                {
                    doesEnemyBlock = false;
                    time = 0.0f;
                }
                state = Nothing;
                break;
        }
    }
}

void DungeonSystem::ReadInDungeonMap(Dungeon& dungeon, std::string file)
{
    Engine::Entity entity = ecsSystem->GetEntity(dungeon);

    dungeon.referenceSpawnTime = Engine::Systems::timeManager->GetTimeSinceStartup();
    RemoveEntityWithChildren(entity, false);

    int width, height, channels;
    std::string path = file;
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
    std::vector<std::vector<bool>> enemyWallMap;
    for(int x = 0; x < width; x++)
    {
        wallMap.push_back(std::vector<bool>());
        enemyWallMap.push_back(std::vector<bool>());
        for(int y = 0; y < height; y++)
        {
            unsigned char red, green, blue, alpha;
            int pos = (((height - y - 1) * width) + x) * 4;
            red =   *(image + pos + 0);
            green = *(image + pos + 1);
            blue =  *(image + pos + 2);
            alpha = *(image + pos + 3);
            Type type1 = ToType(red, green, blue, alpha);

            wallMap[x].push_back(type1 == Type::Wall);
            enemyWallMap[x].push_back((type1 == Type::Wall || type1 == Type::BreakableWall));
            if(type1 == Type::Empty) continue;

            std::string name;
            if(type1 == Type::Wall)
                name = "Wall(";
            else if (type1 == Type::BreakableWall)
                name = "Breakable Wall(";

            name += std::to_string(x);
            name += "|";
            name += std::to_string(y);
            name += ")";

            Engine::Entity wall;
            glm::vec3 spawnPos = glm::vec3(x - width / 2, y - height / 2,0);
            if(type1 == Type::Wall)
                wall = ECSHelper::SpawnWall(spawnPos);
            else if(type1 == Type::BreakableWall)
                wall = ECSHelper::SpawnBreakableWall(spawnPos);

            ecsSystem->GetComponent<Engine::Name>(wall) = name;
            ecsSystem->GetComponent<Engine::Transform>(wall).SetParent(&ecsSystem->GetComponent<Engine::Transform>(entity));
        }
    }
    enemyBehaviourSystem->ChangeWallMap(enemyWallMap);

    if(ecsSystem->HasComponent<Engine::TilemapCollider>(entity))
        ecsSystem->RemoveComponent<Engine::TilemapCollider>(entity);
    Engine::TilemapCollider& collider = ecsSystem->AddComponent<Engine::TilemapCollider>(entity);
    collider.map = wallMap;
    collider.layer = (unsigned char)CollisionLayer::Dungeon;
    collider.position = {wallMap.size() / 2, wallMap[0].size() / 2, 0};
    collider.position *= -1;

}

void DungeonSystem::LoadNextDungeon()
{
    Dungeon& dungeon = ecsSystem->GetComponent<Dungeon>(entity);
    dungeon.activeDungeon++;

    std::filesystem::path enemyFile = dungeon.pathToDungeons / (dungeon.fileName + std::to_string(dungeon.activeDungeon) + ".txt");
    std::filesystem::path dungeonFile = dungeon.pathToDungeons / (dungeon.fileName + std::to_string(dungeon.activeDungeon) + ".png");
    try
    {
        ReadInDungeonMap(dungeon, dungeonFile.string());
        ReadInEnemies(dungeon, enemyFile.string());
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
        exit(-1);
    }
}

DungeonSystem::Type DungeonSystem::ToType(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
{
    static glm::vec4 wall{0, 0, 0, 1};
    static glm::vec4 breakableWall{0, 0, 1, 1};
    static glm::vec4 empty{0, 0, 0, 0};

    glm::vec4 colour = {red, green, blue, alpha};
    colour /= 255.0f;

    Type type1 = Type::Empty;
    float shortestDist = glm::length(empty - colour);

    glm::vec4 dist = wall - colour;
    if(glm::length(dist) < shortestDist)
    {
        type1 = Type::Wall;
        shortestDist = glm::length(dist);
    }

    dist = breakableWall - colour;
    if(glm::length(dist) < shortestDist)
    {
        type1 = Type::BreakableWall;
        shortestDist = glm::length(dist);
    }

    return type1;
}

bool DungeonSystem::IsDungeonCleared()
{
    Dungeon& dungeon = ecsSystem->GetComponent<Dungeon>(entity);
    return dungeon.spawnerData.size() == 0 && dungeon.activeEnemies.size() == 0;
}

void DungeonSystem::OnEnemyDestroyed(Engine::Entity enemy)
{
    Dungeon& dungeon = ecsSystem->GetComponent<Dungeon>(entity);
    dungeon.activeEnemies.remove(enemy);
}

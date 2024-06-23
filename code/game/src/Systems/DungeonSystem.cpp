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

#include "Scenes/02_Game.h"
#include "Scenes/03_GameWin.h"

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
        enemyFile = dungeon.pathToDungeons / (dungeon.fileName + std::to_string(dungeon.activeDungeonIndex) + ".txt");
        dungeonFile = dungeon.pathToDungeons / (dungeon.fileName + std::to_string(dungeon.activeDungeonIndex) + ".png");
    }

    ReadInDungeonMap(dungeonFile.string());
    Systems::enemyBehaviourSystem->UpdateGraph();
    ReadInEnemies(enemyFile.string());
}

void DungeonSystem::Uninitialize()
{
    Engine::RemoveEntityWithChildren(entity);
}


void DungeonSystem::Update()
{
    if(entity == Engine::Entity::INVALID_ENTITY_ID) return;
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
            SpawnEnemy(sceneManager->GetActiveScene<Game>().CreateHubertus(frontEnemy.spawnPosition));
            break;
        case EnemyBehaviour::KindredSpirit:
            SpawnEnemy(sceneManager->GetActiveScene<Game>().CreateKindredSpirit(frontEnemy.spawnPosition).first);
            break;
        case EnemyBehaviour::Assi:
            SpawnEnemy(sceneManager->GetActiveScene<Game>().CreateAssi(frontEnemy.spawnPosition));
            break;
        case EnemyBehaviour::Cuball:
            SpawnEnemy(sceneManager->GetActiveScene<Game>().CreateCuball(frontEnemy.spawnPosition));
            break;
        case EnemyBehaviour::Duke:SpawnEnemy(sceneManager->GetActiveScene<Game>().CreateDuke(frontEnemy.spawnPosition));
            break;
    }
    dungeon.spawnerData.pop_front();
}

void DungeonSystem::SpawnEnemy(Engine::Entity entity)
{
    Dungeon& dungeon = ecsSystem->GetComponent<Dungeon>(this->entity);
    dungeon.activeEnemies.push_back(entity);

    ecsSystem->GetComponent<Engine::BoxCollider>(entity).layer = (int) CollisionLayer::Ignore;
    if (ecsSystem->HasComponent<Cuball>(entity))
    {
        Engine::Systems::animationSystem->PlayAnimation(entity, "Cuball_Spawning");
    }
    else
    {
        Engine::Entity elevator = sceneManager->GetActiveScene<Game>().CreateElevator(ecsSystem->GetComponent<Engine::Transform>(entity).GetTranslation(), entity);
        Engine::Systems::animationSystem->PlayAnimation(elevator, "Elevator_Spawning");

        if(ecsSystem->HasComponent<KindredSpirit>(entity) && ecsSystem->GetComponent<KindredSpirit>(entity).isMainEntity)
        {
            Engine::Entity otherKindredSpirit = ecsSystem->GetComponent<KindredSpirit>(entity).other;
            SpawnEnemy(otherKindredSpirit);
            dungeon.activeEnemies.push_back(otherKindredSpirit);
        }
    }
}


void DungeonSystem::LoadNextDungeon()
{
    Dungeon& dungeon = ecsSystem->GetComponent<Dungeon>(entity);
    dungeon.activeDungeonIndex++;
    Systems::bulletSystem->RemoveAllBullets();
    ecsSystem->GetComponent<PlayerController>(players.first).activeBullets = 0;
    if(ecsSystem->IsEntityActive(players.second))
        ecsSystem->GetComponent<PlayerController>(players.second).activeBullets = 0;

    std::filesystem::path enemyFile = dungeon.pathToDungeons / (dungeon.fileName + std::to_string(dungeon.activeDungeonIndex) + ".txt");
    std::filesystem::path dungeonFile = dungeon.pathToDungeons / (dungeon.fileName + std::to_string(dungeon.activeDungeonIndex) + ".png");
    try
    {
        ReadInDungeonMap(dungeonFile.string());
        Systems::enemyBehaviourSystem->UpdateGraph();
        ReadInEnemies(enemyFile.string());
    }
    catch (std::exception& e)
    {
        Game::scoreP1 = ecsSystem->GetComponent<PlayerController>(players.first).GetScore();
        if(players.second != Engine::Entity::INVALID_ENTITY_ID)
            Game::scoreP2 = ecsSystem->GetComponent<PlayerController>(players.second).GetScore();
        sceneManager->LoadScene<GameWin>();
    }

    ecsSystem->GetComponent<PlayerController>(players.first).stunnedTimer = 0;
    if(players.second != Engine::Entity::INVALID_ENTITY_ID)
        ecsSystem->GetComponent<PlayerController>(players.second).stunnedTimer = 0;
}

void DungeonSystem::ReadInEnemies(std::string file)
{
    static const std::string WAIT_KEYWORD = "wait";
    static const std::string BLOCK_KEYWORD = "block";
    static const std::string SINGLE_KEYWORD = "singleplayer";
    static const std::string MULTI_KEYWORD = "twoplayers";
    static const std::string MIRROR_KEYWORD = "mirror";
    static const std::string PREPPOS_KEYWORD = "preppos";

    Dungeon& dungeon = ecsSystem->GetComponent<Dungeon>(entity);

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
        std::cout << "Game is over, you won, all dungeons cleared. Hurray" << std::endl;
        std::cout << "Seems though as if the developer forgot to handle the case that someone actually finished the game (or played it)" << std::endl;
        exit(0);
    }

    bool doesEnemyBlock = false;
    float time = 0;
    float timeDelta;
    EnemyBehaviour::Type behaviour;
    std::pair<int, int> pos;
    for (int i = 0; i < filecontent.size(); i++)
    {
        std::string &symbol = filecontent[i];
        std::transform(symbol.begin(), symbol.end(), symbol.begin(), tolower);

        if (symbol == WAIT_KEYWORD)
        {
            timeDelta = std::stof(filecontent[i + 1]);
            time += timeDelta;
            i += 1;
        }
        else if (symbol == BLOCK_KEYWORD)
        {
            doesEnemyBlock = true;
        }
        else if (symbol == SINGLE_KEYWORD && players.second == Engine::Entity::INVALID_ENTITY_ID)
        {
            Engine::Transform &transform1 = ecsSystem->GetComponent<Engine::Transform>(players.first);
            std::pair<int, int> pos1 = {std::stoi(filecontent[i + 1]), std::stoi(filecontent[i + 2])};
            transform1.SetTranslation(glm::vec3(ToGlobal(pos1), -0.5f));
            i += 2;
        }
        else if (symbol == MULTI_KEYWORD && players.second != Engine::Entity::INVALID_ENTITY_ID)
        {
            Engine::Transform &transform1 = ecsSystem->GetComponent<Engine::Transform>(players.first);
            std::pair<int, int> pos1 = {std::stoi(filecontent[i + 1]), std::stoi(filecontent[i + 2])};
            transform1.SetTranslation(glm::vec3(ToGlobal(pos1), -0.5f));

            Engine::Transform &transform2 = ecsSystem->GetComponent<Engine::Transform>(players.second);
            std::pair<int, int> pos2 = {std::stoi(filecontent[i + 3]), std::stoi(filecontent[i + 4])};
            transform2.SetTranslation(glm::vec3(ToGlobal(pos2), -0.5f));
            i += 4;
        }
        else if (symbol == MIRROR_KEYWORD)
        {
            std::pair<int, int> mirrors{std::stoi(filecontent[i + 1]), std::stoi(filecontent[i + 2])};
            Cuball::mirrorDirection = mirrors;
            i += 2;
        }
        else if (EnemyBehaviour::stringToBehaviour.count(symbol))
        {
            behaviour = EnemyBehaviour::stringToBehaviour.at(symbol);
            pos.first = std::stoi(filecontent[i + 1]);
            pos.second = std::stoi(filecontent[i + 2]);

            dungeon.spawnerData.push_back({behaviour, pos, time, doesEnemyBlock});
            if (doesEnemyBlock)
            {
                doesEnemyBlock = false;
                time = 0.0f;
            }
            i += 2;
        }
    }
}

void DungeonSystem::ReadInDungeonMap(std::string file)
{
    Dungeon& dungeon = ecsSystem->GetComponent<Dungeon>(entity);

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

    std::vector<std::vector<bool>> wallMap;
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
                name = "Breakable Wall( ";

            name += std::to_string(x) + " | " + std::to_string(y) + " )";

            glm::vec3 spawnPos = glm::vec3(x - width / 2, y - height / 2,0);
            Engine::Entity wall;
            if(type1 == Type::Wall)
                wall = sceneManager->GetActiveScene<Game>().CreateWall(spawnPos);
            else if(type1 == Type::BreakableWall)
                wall = sceneManager->GetActiveScene<Game>().CreateBreakableWall(spawnPos);

            ecsSystem->GetComponent<Engine::Name>(wall) = name;
            ecsSystem->GetComponent<Engine::Transform>(wall).SetParent(&ecsSystem->GetComponent<Engine::Transform>(entity));
        }
    }
    dungeon.wallMap = wallMap;
    dungeon.size = {width, height};
    dungeon.originOffset = glm::vec2(width, height) / -2.0f;
    dungeon.originOffset.x = glm::ceil(dungeon.originOffset.x);
    dungeon.originOffset.y = glm::ceil(dungeon.originOffset.y) * -1;

    Engine::TilemapCollider& collider = ecsSystem->HasComponent<Engine::TilemapCollider>(entity) ? ecsSystem->GetComponent<Engine::TilemapCollider>(entity) :
            ecsSystem->AddComponent<Engine::TilemapCollider>(entity);
    collider.map = wallMap;
    collider.layer = (unsigned char)CollisionLayer::Dungeon;
    collider.position = {wallMap.size() / 2, wallMap[0].size() / 2, 0};
    collider.position *= -1;
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
    if(entity == Engine::Entity::INVALID_ENTITY_ID) return false;
    Dungeon& dungeon = ecsSystem->GetComponent<Dungeon>(entity);
    return dungeon.spawnerData.size() == 0 && dungeon.activeEnemies.size() == 0;
}

void DungeonSystem::OnEnemyDestroyed(Engine::Entity enemy)
{
    if(!ecsSystem->IsEntityActive(entity)) return;
    Dungeon& dungeon = ecsSystem->GetComponent<Dungeon>(entity);
    dungeon.activeEnemies.remove(enemy);
}

void DungeonSystem::ChangeWall(int x, int y, bool isWall)
{
    ecsSystem->GetComponent<Dungeon>(entity).wallMap[x][y] = isWall;
    Systems::enemyBehaviourSystem->UpdateGraph();
}


/// Will crash, if the wallMap does not have a wall on its outer border
bool DungeonSystem::IsNode(std::vector<std::vector<bool>> const &wallMap, int x, int y)
{
    //position is a wall
    if(wallMap[x][y]) return false;
    int connections = 0;
    if(!wallMap[x + 1][y]) connections++;
    if(!wallMap[x - 1][y]) connections++;
    if(!wallMap[x][y + 1]) connections++;
    if(!wallMap[x][y - 1]) connections++;
    //position is not a straight path
    if(connections != 2) return true;
    //position is a straight path
    if((!wallMap[x + 1][y] && !wallMap[x - 1][y]) || (!wallMap[x][y + 1] && !wallMap[x][y - 1])) return false;
    //position is a corner
    return true;
}

bool DungeonSystem::IsWall(std::pair<int, int> pos)
{
    auto const& wallMap = GetWallMap();
    if(pos.first < 0 || pos.second < 0 || pos.first >= wallMap.size() || pos.second >= wallMap[0].size())
        return true;
    return wallMap[pos.first][pos.second];
}

Engine::Entity DungeonSystem::GetDungeonEntity()
{
    return entity;
}

std::pair<int, int> DungeonSystem::GetDungeonSize()
{
    return ecsSystem->GetComponent<Dungeon>(entity).size;
}

std::vector<std::vector<bool>> &DungeonSystem::GetWallMap()
{
    return ecsSystem->GetComponent<Dungeon>(entity).wallMap;
}

glm::vec2 DungeonSystem::GetOriginOffset()
{
    return ecsSystem->GetComponent<Dungeon>(entity).originOffset;
}

/// Transforms the given point, which is in dungeon coordinates, into global coordinates
/// \param dungeonPos
/// \return
glm::vec2 DungeonSystem::ToGlobal(glm::vec2 dungeonPos)
{
    glm::vec2 originOffset = GetOriginOffset();
    return glm::vec2(originOffset.x + dungeonPos.x, originOffset.y - dungeonPos.y);
}


glm::vec2 DungeonSystem::ToGlobal(std::pair<int, int> dungeonPos)
{
    glm::vec2 originOffset = GetOriginOffset();
    return glm::vec2(originOffset.x + dungeonPos.first, originOffset.y - dungeonPos.second);
}

std::pair<int, int> DungeonSystem::ToDungeon(glm::vec3 globalPos)
{
    glm::vec2 originOffset = GetOriginOffset();
    return {glm::round(globalPos.x - originOffset.x), glm::round(originOffset.y - globalPos.y)};
}

std::pair<int, int> DungeonSystem::GetRandomFreePos(int startX, int startY, int radius, int tries)
{
    Dungeon& dungeon = ecsSystem->GetComponent<Dungeon>(entity);
    if(startX == INT_MIN)
        startX = dungeon.wallMap.size() / 2;
    if (startY == INT_MIN)
        startY = dungeon.wallMap[0].size() / 2;
    if (radius == INT_MIN)
        radius = (std::min(dungeon.wallMap.size(), dungeon.wallMap[0].size())) / 2;

    int x, y;
    while(tries > 0)
    {
        x = startX + (rand() % (radius * 2 + 1) - radius);
        y = startY + (rand() % (radius * 2 + 1) - radius);

        if(!IsWall({x, y}))
            return {x, y};
        tries--;
    }
    return {-1, -1};
}

#pragma once
#include "ECSSystem.h"
#include "Components/Dungeon.h"

class DungeonSystem : public Engine::System
{
    enum class Type
    {
        Empty,
        Wall,
        BreakableWall
    };

    Engine::Entity entity = Engine::Entity::INVALID_ENTITY_ID;

    void EntityAdded(Engine::Entity entity) override;
    void EntityRemoved(Engine::Entity entity) override;

    void ReadInDungeonMap(std::string file);
    void ReadInEnemies(std::string file);

    Type ToType(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);
public:
    void Initialize();
    void Update();
    void LoadNextDungeon();
    bool IsDungeonCleared();
    void OnEnemyDestroyed(Engine::Entity enemy);

    void ChangeWall(int x, int y, bool isWall);
    bool IsWall(std::pair<int, int> pos);
    bool IsNode(std::vector<std::vector<bool>> const &wallMap, int x, int y);
    std::pair<int, int> GetRandomFreePos();

    Engine::Entity GetDungeonEntity();
    std::pair<int, int> GetDungeonSize();
    std::vector<std::vector<bool>>& GetWallMap();
    glm::vec2 GetOriginOffset();

    glm::vec2 ToGlobal(glm::vec2 dungeonPos);
    glm::vec2 ToGlobal(std::pair<int, int> dungeonPos);
    std::pair<int, int> ToDungeon(glm::vec3 globalPos);
};
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

    Engine::Entity entity;

    void EntityAdded(Engine::Entity entity) override;
    void EntityRemoved(Engine::Entity entity) override;

    void ReadInDungeonMap(Dungeon& dungeon, std::string file);
    void ReadInEnemies(Dungeon& dungeon, std::string file);

    Type ToType(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);
public:
    std::vector<std::vector<bool>> wallMap;

    void Initialize();
    void Update();
    void LoadNextDungeon();
    bool IsDungeonCleared();
    void OnEnemyDestroyed(Engine::Entity enemy);

};
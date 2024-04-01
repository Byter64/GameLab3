#pragma once
#include "ECSSystem.h"

class DungeonSystem : public Engine::System
{
    void EntityAdded(Engine::Entity entity) override;
    void EntityRemoved(Engine::Entity entity) override;

    void ReadInDungeonMap(Engine::Entity entity);
    void ReadInEnemies(Engine::Entity entity);
    bool TryLoadTestDungeonAndEnemies(Engine::Entity entity);
public:
    std::vector<std::vector<bool>> wallMap;

    void InitializeDungeons();
    void Update();
    void UpdateDungeon(Engine::Entity entity);
};
#pragma once
#include "ECSSystem.h"
#include "Systems/DungeonSystem.h"

class DungeonEnemySystem : public Engine::System
{
    void EntityAdded(Engine::Entity entity) override;
    void EntityRemoved(Engine::Entity entity) override;
};

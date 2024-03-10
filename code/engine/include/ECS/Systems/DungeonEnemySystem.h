#pragma once
#include "ECSSystem.h"
#include "Systems/DungeonSystem.h"
namespace Engine
{

    class DungeonEnemySystem : public System
    {
        void EntityAdded(Entity entity) override;
        void EntityRemoved(Entity entity) override;
    };

} // Engine

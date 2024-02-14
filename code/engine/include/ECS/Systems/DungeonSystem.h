#pragma once
#include "ECSSystem.h"
namespace Engine
{

    class DungeonSystem : public System
    {
        void EntityAdded(Entity entity) override;
        void EntityRemoved(Entity entity) override;

        void ReadInNewDungeon(Entity entity);
    public:
        void Update();
    };

} // Engine

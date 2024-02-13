#pragma once
#include "ECSSystem.h"
namespace Engine
{

    class DungeonSystem : public System
    {
        void EntityAdded(Entity entity) override;
        void EntityRemoved(Entity entity) override;

    public:
        void Update();
    };

} // Engine

#pragma once
#include "ECSSystem.h"
namespace Engine
{

    class DungeonSystem : public System
    {
        void EntityAdded(Entity entity) override;
        void EntityRemoved(Entity entity) override;

        void ReadInDungeonMap(Entity entity);
        void ReadInEnemies(Entity entity);
    public:
        std::vector<std::vector<bool>> wallMap;

        void Update();
    };

} // Engine

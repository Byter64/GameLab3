#pragma once
#include "ECS/Components/EnemyBehaviour.h"
#include "ECS/System.h"
#include <vector>
#include <queue>

namespace Engine
{
    class EnemyBehaviourSystem : public System
    {

        void EntityAdded(Entity entity);
        void EntityRemoved(Entity entity);

        bool DoesPositionExistAndIsFree(int x, int y);
        std::queue<std::pair<int, int>> GetShuffeldAdjacentFields(int x, int y);
        void Update1(Entity entity, float deltaTime);

    public:
        /*
        * outer dimension is x, inner is y
        */
        std::vector<std::vector<bool>> dungeonMap;

        EnemyBehaviourSystem();

        void Update(float deltaTime);
    };
}
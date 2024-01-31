#pragma once
#include "ECS/Components/EnemyBehaviour.h"
#include "ECS/System.h"
#include <vector>
#include <queue>
#include <list>
#include <map>

namespace Engine
{
    class EnemyBehaviourSystem : public System
    {
        std::vector<std::vector<bool>> wallMap;
        std::map<std::pair<int, int>, std::list<std::pair<int, int>>> graph;
        std::pair<int, int> dungeonSize;

        void EntityAdded(Entity entity);
        void EntityRemoved(Entity entity);

        std::pair<int, int> FindNode(int startx, int starty, int dirx, int diry);
        bool IsNode(std::vector<std::vector<bool>>& wallMap, int x, int y);
        bool DoesPositionExistAndIsFree(int x, int y);
        std::queue<std::pair<int, int>> GetShuffeldAdjacentFields(int x, int y);
        void Update1(Entity entity, float deltaTime);

    public:
        /*
        * outer dimension is x, inner is y
        */

        EnemyBehaviourSystem();
        void Initialize(std::vector<std::vector<bool>>& wallMap);
        void Update(float deltaTime);
    };
}
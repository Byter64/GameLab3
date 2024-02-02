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
        glm::vec2 originOffset;

        void EntityAdded(Entity entity);
        void EntityRemoved(Entity entity);

        std::pair<int, int> FindNode(int startx, int starty, int dirx, int diry);
        bool IsNode(std::vector<std::vector<bool>>& wallMap, int x, int y);

        //Stuff for Hubertus
        std::pair<float, float> idleDurationRange = std::make_pair(1.0f, 4.0f);
        std::pair<float, float> idleTimeRange = std::make_pair(10.0f, 30.0f);
        std::pair<float, float> shootTimeRange = std::make_pair(2.0f, 8.0f);
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
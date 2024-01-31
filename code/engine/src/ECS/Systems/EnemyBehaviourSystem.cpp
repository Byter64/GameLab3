#include "ECS/Systems/EnemyBehaviourSystem.h"
#include "Engine.h"
#include <chrono>
#include <iostream>
#include <list>
#include <cstdlib>
#include <iterator>
#include <random>

namespace Engine
{
    EnemyBehaviourSystem::EnemyBehaviourSystem()
    {
        srand(std::chrono::system_clock::now().time_since_epoch().count());
    }

    void EnemyBehaviourSystem::EntityAdded(Entity entity)
    {
        EnemyBehaviour &behaviour = ecsSystem->GetComponent<EnemyBehaviour>(entity);
        Transform& transform = ecsSystem->GetComponent<Transform>(entity);

        int index = rand() % graph.size();
        auto iter = graph.begin();
        std::advance(iter, index);

        behaviour.targetNode = iter->first;
        behaviour.targetPos = glm::vec2(behaviour.targetNode.first, behaviour.targetNode.second);
        transform.SetTranslation(glm::vec3(behaviour.targetPos, 0));
    }

    void EnemyBehaviourSystem::EntityRemoved(Entity entity)
    {

    }


    void EnemyBehaviourSystem::Update(float deltaTime)
    {
        //Each entity must have an "EnemyBehaviour", "Health" and "Collider"
        //Wie weiß der Gegner, wo er ist??
        for (Entity entity: entities)
        {
            EnemyBehaviour &behaviour = ecsSystem->GetComponent<EnemyBehaviour>(entity);

            switch (behaviour.behaviour)
            {
                default:
                case EnemyBehaviour::Default:
                    Update1(entity, deltaTime);
                    break;
            }
        }
    }

    void EnemyBehaviourSystem::Update1(Entity entity, float deltaTime)
    {
        EnemyBehaviour &behaviour = ecsSystem->GetComponent<EnemyBehaviour>(entity);
        Transform &transform = ecsSystem->GetComponent<Transform>(entity);

        if(glm::length(behaviour.targetPos - glm::vec2(transform.GetGlobalTranslation())) < behaviour.movementSpeed * deltaTime)
        {
            std::list<std::pair<int, int>>& list = graph[behaviour.targetNode];
            int index = rand() % list.size();
            auto iter = list.begin();
            std::advance(iter, index);

            auto oldTarget = behaviour.targetNode;
            behaviour.targetNode = *iter;
            behaviour.targetPos = glm::vec2(behaviour.targetNode.first, behaviour.targetNode.second);
            behaviour.movement =  glm::normalize(glm::vec2(behaviour.targetNode.first, behaviour.targetNode.second) -  glm::vec2(oldTarget.first, oldTarget.second));
        }

        std::random_device rd;
        std::mt19937 gen(rd());

        if(behaviour.shootTimer <= 0)
        {
            std::cout << "I made a shot" << std::endl;
            std::uniform_real_distribution<> distr(shootTimeRange.first, shootTimeRange.second);
            behaviour.shootTimer = distr(gen);
        }

        if(behaviour.idlerTimer <= 0)
        {
            std::uniform_real_distribution<> distr;
            if(behaviour.isMoving)
                distr = std::uniform_real_distribution<>(idleDurationRange.first, idleDurationRange.second);
            else
                distr = std::uniform_real_distribution<>(idleTimeRange.first, idleTimeRange.second);

            behaviour.isMoving = !behaviour.isMoving;
            behaviour.idlerTimer = distr(gen);
        }

        behaviour.idlerTimer -= deltaTime;
        behaviour.shootTimer -= deltaTime;

        if(behaviour.isMoving)
            transform.AddTranslation(glm::vec3(behaviour.movement * behaviour.movementSpeed * deltaTime, 0));
    }

    std::pair<int, int> EnemyBehaviourSystem::FindNode(int startx, int starty, int dirx, int diry)
    {
        startx += dirx;
        starty += diry;
        while(startx < dungeonSize.first && startx >= 0 && starty < dungeonSize.second && starty >= 0)
        {
            if(graph.count(std::make_pair(startx, starty)))
            {
                return std::make_pair(startx, starty);
            }

            startx += dirx;
            starty += diry;
        }
        return std::make_pair(-1, -1);
    }

    /// This needs to be called before the first call to Update
    /// \param wallMap a bool map of the dungeon, where true marks a wall, a false marks free space
    void EnemyBehaviourSystem::Initialize(std::vector<std::vector<bool>>& wallMap)
    {
        dungeonSize = std::make_pair(wallMap.size(), wallMap[0].size());
        //Generate all nodes
        for (int y = 0; y < wallMap[0].size(); y++)
        {
            for (int x = 0; x < wallMap.size(); x++)
            {
                if (IsNode(wallMap, x, y))
                {
                    graph[std::make_pair(x, y)] = std::list<std::pair<int, int>>();
                }
            }
        }

        //Create connections between nodes
        for (auto& pair : graph)
        {
            std::pair<int, int> pos = pair.first;
            std::pair<int, int> top = FindNode(pos.first, pos.second, 0, 1);
            std::pair<int, int> bottom = FindNode(pos.first, pos.second, 0, -1);
            std::pair<int, int> left = FindNode(pos.first, pos.second, -1, 0);
            std::pair<int, int> right = FindNode(pos.first, pos.second, 1, 0);

            if(top.first != -1)
                pair.second.push_back(top);
            if(bottom.first != -1)
                pair.second.push_back(bottom);
            if(left.first != -1)
                pair.second.push_back(left);
            if(right.first != -1)
                pair.second.push_back(right);
        }
    }

    /// Will crash, if the wallMap does not have a wall on its outer border
    bool EnemyBehaviourSystem::IsNode(std::vector<std::vector<bool>> &wallMap, int x, int y)
    {
        //position is a wall
        if(wallMap[x][y]) return false;

        int connections = 0;
        if(!wallMap[x + 1][y]) connections++;
        if(!wallMap[x - 1][y]) connections++;
        if(!wallMap[x][y + 1]) connections++;
        if(!wallMap[x][y - 1]) connections++;

        //position is not a straight path
        if(connections != 2) return true;

        //position is a straight path
        if((!wallMap[x + 1][y] && !wallMap[x - 1][y]) || (!wallMap[x][y + 1] && !wallMap[x][y - 1])) return false;

        //position is a corner
        return true;
    }
}
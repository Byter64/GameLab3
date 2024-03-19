#pragma once
#include "Components/EnemyBehaviour.h"
#include "ECS/System.h"
#include <vector>
#include <queue>
#include <list>
#include <map>

class EnemyBehaviourSystem : public Engine::System
{
    std::vector<std::vector<bool>> wallMap;
    std::map<std::pair<int, int>, std::list<std::pair<int, int>>> graph;
    std::pair<int, int> dungeonSize;
    glm::vec2 originOffset;

    void EntityAdded(Engine::Entity entity);
    void EntityRemoved(Engine::Entity entity);

    std::pair<int, int> FindNode(int startx, int starty, int dirx, int diry);
    bool IsNode(std::vector<std::vector<bool>>& wallMap, int x, int y);
    glm::vec2 ToGlobal(glm::vec2 dungeonPos);
    glm::vec2 ToGlobal(std::pair<int, int> dungeonPos);

    //Stuff for Hubertus
    std::pair<float, float> idleDurationRange = std::make_pair(1.0f, 4.0f);
    std::pair<float, float> idleTimeRange = std::make_pair(10.0f, 30.0f);
    std::pair<float, float> shootTimeRange = std::make_pair(2.0f, 8.0f);
    void UpdateHubertus(Engine::Entity entity, float deltaTime);
    void HandleDamageHubertus(Engine::Entity entity, Engine::Entity other);
    void UpdateKindredSpirit(Engine::Entity entity, float deltaTime);
    void HandleDamageKindredSpirit(Engine::Entity entity, Engine::Entity other);
    void MoveEnemy(EnemyBehaviour& behaviour, Engine::Transform& transform, float deltaTime);
public:

    EnemyBehaviourSystem();
    void Initialize(std::vector<std::vector<bool>>& wallMap);
    void Update(float deltaTime);
};
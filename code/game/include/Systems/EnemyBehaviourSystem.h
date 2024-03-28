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
    std::vector<std::pair<int, int>> FindNodes(int startx, int starty);
    std::pair<int, int> FindWall(int startx, int starty, int dirx, int diry);

    bool IsNode(std::vector<std::vector<bool>> &wallMap, int x, int y);

    glm::vec2 ToGlobal(glm::vec2 dungeonPos);
    glm::vec2 ToGlobal(std::pair<int, int> dungeonPos);
    std::pair<int, int> ToDungeon(glm::vec3 globalPos);

    inline static float enemyScoreDecrease;

    inline static std::map<EnemyBehaviour::Type, std::pair<float, float>> idleDurationRanges;
    inline static std::map<EnemyBehaviour::Type, std::pair<float, float>> walkDurationRanges;
    inline static std::map<EnemyBehaviour::Type, std::pair<float, float>> shootIntervalRanges;

    void UpdateHubertus(Engine::Entity entity, float deltaTime);
    void HandleDamageHubertus(Engine::Entity entity, Engine::Entity other);

    void UpdateKindredSpirit(Engine::Entity entity, float deltaTime);
    void HandleDamageKindredSpirit(Engine::Entity entity, Engine::Entity other);

    void UpdateAssi(Engine::Entity entity, float deltaTime);
    void HandleDamageAssi(Engine::Entity entity, Engine::Entity other);

    void MoveEnemyNormal(EnemyBehaviour& behaviour, Engine::Transform& transform, float deltaTime);
    void MoveAssi(EnemyBehaviour& behaviour, Engine::Transform& transform, glm::vec3 direction, float deltaTime);

    Engine::Entity FindPlayerInSight(Engine::Entity enemy, int maxDistance);
public:

    EnemyBehaviourSystem();
    void Initialize(std::vector<std::vector<bool>>& wallMap);
    void ChangeWallMap(std::vector<std::vector<bool>>& wallMap);
    void Update(float deltaTime);
};
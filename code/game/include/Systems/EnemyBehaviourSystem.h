#pragma once
#include "Components/EnemyBehaviour.h"
#include "ECS/System.h"
#include <vector>
#include <queue>
#include <list>
#include <map>
#include "AStar.hpp"

class EnemyBehaviourSystem : public Engine::System
{
    inline static float enemyScoreDecrease;
    inline static std::map<EnemyBehaviour::Type, std::pair<float, float>> idleDurationRanges;
    inline static std::map<EnemyBehaviour::Type, std::pair<float, float>> walkDurationRanges;
    inline static std::map<EnemyBehaviour::Type, std::pair<float, float>> shootIntervalRanges;

    AStar::Generator generator;
    std::map<std::pair<int, int>, std::list<std::pair<int, int>>> graph;

    void EntityAdded(Engine::Entity entity);
    void EntityRemoved(Engine::Entity entity);

    std::pair<int, int> FindNode(int startx, int starty, int dirx, int diry);
    std::vector<std::pair<int, int>> FindNodes(int startx, int starty);
    std::pair<int, int> FindWall(int startx, int starty, int dirx, int diry);

    bool IsWall(std::pair<int, int> pos);
    bool IsNode(std::vector<std::vector<bool>> const &wallMap, int x, int y);

    void UpdateHubertus(Engine::Entity entity, float deltaTime);
    void HandleDamageHubertus(Engine::Entity entity, Engine::Entity other);

    void UpdateKindredSpirit(Engine::Entity entity, float deltaTime);
    void HandleDamageKindredSpirit(Engine::Entity entity, Engine::Entity other);

    void UpdateAssi(Engine::Entity entity, float deltaTime);
    void HandleDamageAssi(Engine::Entity entity, Engine::Entity other);

    void UpdateCuball(Engine::Entity entity, float deltaTime);
    void HandleDamageCuball(Engine::Entity entity, Engine::Entity other);

    void UpdateDuke(Engine::Entity entity, float deltaTime);
    void HandleDamageDuke(Engine::Entity entity, Engine::Entity other);

    void SetTarget(EnemyBehaviour& behaviour, Engine::Transform& transform, std::pair<int, int> dungeonPosStart, std::pair<int, int> dungeonPosTarget, bool setRotation);
    void MoveEnemyNormal(EnemyBehaviour& behaviour, Engine::Transform& transform, float deltaTime, bool setRotation = true);
    void MoveAssi(EnemyBehaviour& behaviour, Engine::Transform& transform, glm::vec3 direction, float deltaTime);
    void MoveCuball(EnemyBehaviour& behaviour, Engine::Transform& transform, float deltaTime);
    void MoveDuke(EnemyBehaviour& behaviour, Engine::Transform& transform, float deltaTime);

    Engine::Entity FindPlayerInSight(Engine::Entity enemy, int maxDistance);
    std::vector<std::pair<int, int>> GeneratePath(std::pair<int, int> start, std::pair<int, int> end);
    Engine::Entity FindClosestPlayer(Engine::Entity enemy);

public:
    EnemyBehaviourSystem();

    void UpdateGraph();
    void Update(float deltaTime);

    static glm::vec2 ToGlobal(glm::vec2 dungeonPos);
    static glm::vec2 ToGlobal(std::pair<int, int> dungeonPos);
    static std::pair<int, int> ToDungeon(glm::vec3 globalPos);
};
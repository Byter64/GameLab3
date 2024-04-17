#pragma once
#include "Components/Enemies/EnemyBehaviour.h"
#include "ECS/System.h"
#include <vector>
#include <queue>
#include <list>
#include <map>
#include "AStar.hpp"
#include "Helpers/Movement.h"

class EnemyBehaviourSystem : public Engine::System
{

    AStar::Generator generator;
    std::map<std::pair<int, int>, std::list<std::pair<int, int>>> graph;

    void EntityAdded(Engine::Entity entity);
    void EntityRemoved(Engine::Entity entity);

    void UpdateAssi(Engine::Entity entity, float deltaTime);
    void HandleDamageAssi(Engine::Entity entity, Engine::Entity other);

    void UpdateCuball(Engine::Entity entity, float deltaTime);
    void HandleDamageCuball(Engine::Entity entity, Engine::Entity other);

    void UpdateDuke(Engine::Entity entity, float deltaTime);
    void HandleDamageDuke(Engine::Entity entity, Engine::Entity other);

    void MoveAssi(EnemyBehaviour& behaviour, Engine::Transform& transform, glm::vec3 direction, float deltaTime);
    void MoveCuball(EnemyBehaviour& behaviour, Engine::Transform& transform, float deltaTime);
    void MoveDuke(EnemyBehaviour& behaviour, Engine::Transform& transform, float deltaTime);

    Engine::Entity FindPlayerInSight(Engine::Entity enemy, int maxDistance);
    std::vector<std::pair<int, int>> GeneratePath(std::pair<int, int> start, std::pair<int, int> end);
    Engine::Entity FindClosestPlayer(Engine::Entity enemy);

public:
    EnemyBehaviourSystem();

    std::pair<int, int> FindNode(int startx, int starty, int dirx, int diry);
    std::vector<std::pair<int, int>> FindNodes(int startx, int starty);
    std::pair<int, int> FindWall(int startx, int starty, int dirx, int diry);

    void SetTarget(Movement& movement, std::pair<int, int> dungeonPosStart, std::pair<int, int> dungeonPosTarget);
    void MoveRandomly(Movement& movement, float distance);

    void UpdateGraph();
    void Update(float deltaTime);

    void Kill(Engine::Entity entity);
};
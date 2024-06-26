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

public:
    EnemyBehaviourSystem();

    std::pair<int, int> FindNode(int startx, int starty, int dirx, int diry);
    std::vector<std::pair<int, int>> FindNodes(int startx, int starty);
    std::pair<int, int> FindWall(int startx, int starty, int dirx, int diry);

    std::list<std::pair<int, int>> GetNeighbours(std::pair<int, int> node);
    void SetTarget(Movement& movement, std::pair<int, int> dungeonPosStart, std::pair<int, int> dungeonPosTarget);
    void MoveRandomly(Movement& movement, float distance);
    void MoveStraight(Movement& movement, glm::vec2 direction, float distance);

    void UpdateGraph();

    Engine::Entity FindPlayerInSight(Engine::Entity enemy, int maxDistance);
    Engine::Entity FindPlayerInSight(glm::vec3 globalPosition, int maxDistance);
    std::vector<std::pair<int, int>> GeneratePath(std::pair<int, int> start, std::pair<int, int> end);
    Engine::Entity FindClosestPlayer(Engine::Entity enemy);

    void Kill(Engine::Entity entity, int score);
};
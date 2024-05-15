#include "Systems/EnemyBehaviourSystem.h"
#include "Engine.h"
#include <chrono>
#include <iostream>
#include <list>
#include <cstdlib>
#include <iterator>
#include <random>
#include "ECSExtension.h"
#include <cmath>
#include "GameDefines.h"
#include <limits>

extern std::pair<Engine::Entity, Engine::Entity> players;

EnemyBehaviourSystem::EnemyBehaviourSystem()
{
    EnemyBehaviour::scores[EnemyBehaviour::Hubertus] = Defines::Int("Hubertus_Score");
    EnemyBehaviour::scores[EnemyBehaviour::KindredSpirit] = Defines::Int("KindredSpirit_Score");
    EnemyBehaviour::scores[EnemyBehaviour::Assi] = Defines::Int("Assi_Score");
    EnemyBehaviour::scores[EnemyBehaviour::Cuball] = Defines::Int("Cuball_Score");
    EnemyBehaviour::scores[EnemyBehaviour::Duke] = Defines::Int("Duke_Score");

    EnemyBehaviour::scoreDecrease = Defines::Float("Enemy_ScoreDecrease");

    generator.setHeuristic(AStar::Heuristic::euclidean);
    generator.setDiagonalMovement(false);
}

void EnemyBehaviourSystem::EntityAdded(Engine::Entity entity)
{

}

void EnemyBehaviourSystem::EntityRemoved(Engine::Entity entity)
{
    Systems::dungeonSystem->OnEnemyDestroyed(entity);
}

std::list<std::pair<int, int>> EnemyBehaviourSystem::GetNeighbours(std::pair<int, int> node)
{
    return graph.at(node);
}

void EnemyBehaviourSystem::SetTarget(Movement &movement, std::pair<int, int> dungeonPosStart, std::pair<int, int> dungeonPosTarget)
{
    movement.oldTargetNode = dungeonPosStart;
    movement.targetNode = dungeonPosTarget;
    movement.targetPos = Systems::dungeonSystem->ToGlobal(movement.targetNode);
    movement.direction = Systems::dungeonSystem->ToGlobal(movement.targetNode) - Systems::dungeonSystem->ToGlobal(movement.oldTargetNode);
    if(movement.direction != glm::vec2(0)) movement.direction = glm::normalize(movement.direction);
}

void EnemyBehaviourSystem::MoveRandomly(Movement& movement, float distance)
{
    float oldDistance = glm::length(movement.targetPos - glm::vec2(movement.currentPos));
    movement.currentPos += movement.direction * distance;
    float newDistance = glm::length(movement.targetPos - glm::vec2(movement.currentPos));
    if(newDistance >= oldDistance)
    {
        std::list<std::pair<int, int>>& list = graph[movement.targetNode];
        auto iter = list.end();
        int i = 0;
        do
        {
            int index = rand() % list.size();
            iter = list.begin();
            std::advance(iter, index);
            i++;
        } while(i < 5 && *iter == movement.oldTargetNode);
        SetTarget(movement, movement.targetNode, *iter);
        movement.currentPos = Systems::dungeonSystem->ToGlobal(movement.oldTargetNode);
    }
}

void EnemyBehaviourSystem::MoveStraight(Movement& movement, glm::vec2 direction, float distance)
{
    if(direction != glm::vec2(0))
        direction = glm::normalize(direction);

    movement.direction = direction;
    movement.currentPos += direction * distance;
}

/// Finds a node by raytracing into a given direction from a given point
/// \param startx
/// \param starty
/// \param dirx
/// \param diry
/// \return
std::pair<int, int> EnemyBehaviourSystem::FindNode(int startx, int starty, int dirx, int diry)
{
    std::pair<int, int> dungeonSize = Systems::dungeonSystem->GetDungeonSize();
    auto const& wallMap = Systems::dungeonSystem->GetWallMap();

    startx += dirx;
    starty += diry;
    while(startx < dungeonSize.first && startx >= 0 && starty < dungeonSize.second && starty >= 0)
    {
        if(wallMap[startx][starty]) break;
        if(graph.count(std::make_pair(startx, starty)))
        {
            return std::make_pair(startx, starty);
        }
        startx += dirx;
        starty += diry;
    }
    return {-1, -1};
}


std::vector<std::pair<int, int>> EnemyBehaviourSystem::FindNodes(int startx, int starty)
{
    std::vector<std::pair<int, int>> targetNodes;
    auto target = FindNode(startx, starty, 1, 0);
    if(target.first != -1) targetNodes.push_back(target);
    target = FindNode(startx, starty,-1, 0);
    if(target.first != -1) targetNodes.push_back(target);
    target = FindNode(startx, starty, 0, 1);
    if(target.first != -1) targetNodes.push_back(target);
    target = FindNode(startx, starty, 0, -1);
    if(target.first != -1) targetNodes.push_back(target);

    return targetNodes;
}

/// Finds a wall by raytracing into a given direction from a given point
/// \param startx
/// \param starty
/// \param dirx
/// \param diry
/// \return
std::pair<int, int> EnemyBehaviourSystem::FindWall(int startx, int starty, int dirx, int diry)
{
    std::pair<int, int> dungeonSize = Systems::dungeonSystem->GetDungeonSize();
    auto const& wallMap = Systems::dungeonSystem->GetWallMap();

    startx += dirx;
    starty += diry;
    while(startx < dungeonSize.first && startx >= 0 && starty < dungeonSize.second && starty >= 0)
    {
        if(wallMap[startx][starty]) return {startx, starty};
        startx += dirx;
        starty += diry;
    }
    return std::make_pair(-1, -1);
}

/// This needs to be called (at least) once before the first call to Update
void EnemyBehaviourSystem::UpdateGraph()
{
    Engine::Entity entity = Systems::dungeonSystem->GetDungeonEntity();
    Dungeon& dungeon = ecsSystem->GetComponent<Dungeon>(entity);
    auto const& wallMap = dungeon.wallMap;
    auto const& dungeonSize = dungeon.size;
    auto const& originOffset = dungeon.originOffset;

    graph.clear();

    generator.clearCollisions();
    generator.setWorldSize({(int)wallMap.size(), (int)wallMap[0].size()});
    for(int x = 0; x < wallMap.size(); x++)
        for(int y = 0; y < wallMap[0].size(); y++)
            if(wallMap[x][y])
                generator.addCollision({x, y});

    //Generate all nodes
    for (int y = 0; y < wallMap[0].size(); y++)
    {
        for (int x = 0; x < wallMap.size(); x++)
        {
            if (Systems::dungeonSystem->IsNode(wallMap, x, y))
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

Engine::Entity EnemyBehaviourSystem::FindPlayerInSight(Engine::Entity enemy, int maxDistance)
{
    Engine::Transform& enemyTransform = ecsSystem->GetComponent<Engine::Transform>(enemy);
    glm::vec3 enemyPos = enemyTransform.GetGlobalTranslation();
    return FindPlayerInSight(enemyPos, maxDistance);
}

Engine::Entity EnemyBehaviourSystem::FindPlayerInSight(glm::vec3 globalPosition, int maxDistance)
{
    std::pair<int, int> enemyPosDungeon = Systems::dungeonSystem->ToDungeon(globalPosition);
    for(Engine::Entity player : Systems::playerControllerSystem->entities)
    {
        glm::vec3 distanceToPlayer = ecsSystem->GetComponent<Engine::Transform>(player).GetGlobalTranslation() - globalPosition;
        glm::vec3 direction = Miscellaneous::RoundTo4Directions(glm::normalize(distanceToPlayer));

        if(length(direction - glm::normalize(distanceToPlayer)) > 0.1f) continue;

        std::pair<int, int> wall = FindWall(enemyPosDungeon.first, enemyPosDungeon.second, (int)direction.x, (int)-direction.y);
        glm::vec3 distanceToWall = glm::vec3(Systems::dungeonSystem->ToGlobal(wall), 0) - globalPosition;
        if(glm::length(distanceToPlayer) < maxDistance && glm::length(distanceToPlayer) < glm::length(distanceToWall))
        {
            return player;
        }
    }

    return Engine::Entity::INVALID_ENTITY_ID;
}


std::vector<std::pair<int, int>> EnemyBehaviourSystem::GeneratePath(std::pair<int, int> start, std::pair<int, int> end)
{
    std::vector<AStar::Vec2i> result = generator.findPath({start.first, start.second}, {end.first, end.second});
    std::vector<std::pair<int, int>>& resultPointer = *(std::vector<std::pair<int, int>>*)(void*)&result;

    return resultPointer;
}

Engine::Entity EnemyBehaviourSystem::FindClosestPlayer(Engine::Entity enemy)
{
    if(players.second == Engine::Entity::INVALID_ENTITY_ID)
        return players.first;

    glm::vec3 distance1 = ecsSystem->GetComponent<Engine::Transform>(players.first).GetGlobalTranslation() - ecsSystem->GetComponent<Engine::Transform>(enemy).GetGlobalTranslation();
    glm::vec3 distance2 = ecsSystem->GetComponent<Engine::Transform>(players.second).GetGlobalTranslation() - ecsSystem->GetComponent<Engine::Transform>(enemy).GetGlobalTranslation();

    if(glm::length(distance1) < glm::length(distance2))
        return players.first;
    else
        return players.second;
}

void EnemyBehaviourSystem::Kill(Engine::Entity entity, int score)
{
    EnemyBehaviour& behaviour = ecsSystem->GetComponent<EnemyBehaviour>(entity);

    RemoveEntityWithChildren(entity);
    float timeAlive = Engine::Systems::timeManager->GetTimeSinceStartup() - behaviour.spawnTime;
    score = score - (int)(timeAlive * EnemyBehaviour::scoreDecrease);
    score = score < 1 ? 1 : score;
    ECSHelper::SpawnLoot(ecsSystem->GetComponent<Engine::Transform>(entity).GetGlobalTranslation(), score);
}

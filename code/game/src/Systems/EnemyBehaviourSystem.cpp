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

EnemyBehaviourSystem::EnemyBehaviourSystem()
{
    srand(std::chrono::system_clock::now().time_since_epoch().count());
}

void EnemyBehaviourSystem::EntityAdded(Engine::Entity entity)
{
    EnemyBehaviour &behaviour = ecsSystem->GetComponent<EnemyBehaviour>(entity);
    Engine::Transform& transform = ecsSystem->GetComponent<Engine::Transform>(entity);

    if(behaviour.behaviour == EnemyBehaviour::KindredSpirit && behaviour.enemyExtra.kindredSpirit.isMainEntity == false)
        return;

    std::vector<std::pair<int,int>> targetNodes;
    auto target = FindNode(behaviour.startPos.first, behaviour.startPos.second, 1, 0);
    if(target.first != -1) targetNodes.push_back(target);
    target = FindNode(behaviour.startPos.first, behaviour.startPos.second, -1, 0);
    if(target.first != -1) targetNodes.push_back(target);
    target = FindNode(behaviour.startPos.first, behaviour.startPos.second, 0, 1);
    if(target.first != -1) targetNodes.push_back(target);
    target = FindNode(behaviour.startPos.first, behaviour.startPos.second, 0, -1);
    if(target.first != -1) targetNodes.push_back(target);
    target = targetNodes[rand() % targetNodes.size()];
    behaviour.oldTargetNode = behaviour.startPos;
    behaviour.targetNode = target;
    behaviour.targetPos = ToGlobal(behaviour.targetNode);
    behaviour.movement = glm::normalize(ToGlobal(behaviour.targetNode) -  ToGlobal(behaviour.oldTargetNode));
    transform.SetTranslation(glm::vec3(ToGlobal(behaviour.startPos), 0));
    transform.SetRotation(glm::quat(glm::vec3(glm::radians(90.0f), 0, glm::atan(behaviour.movement.y, behaviour.movement.x))));
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> distr(shootTimeRange.first, shootTimeRange.second);
    behaviour.shootTimer = distr(gen);
    distr = std::uniform_real_distribution<>(idleTimeRange.first, idleTimeRange.second);
    behaviour.idlerTimer = distr(gen);
    behaviour.isMoving = true;
}

void EnemyBehaviourSystem::EntityRemoved(Engine::Entity entity)
{

}

void EnemyBehaviourSystem::Update(float deltaTime)
{
    for (Engine::Entity entity: entities)
    {
        EnemyBehaviour &behaviour = ecsSystem->GetComponent<EnemyBehaviour>(entity);
        switch (behaviour.behaviour)
        {
            default:
            case EnemyBehaviour::Hubertus:
                UpdateHubertus(entity, deltaTime);
                break;
            case EnemyBehaviour::KindredSpirit:
                UpdateKindredSpirit(entity, deltaTime);
                break;
        }
        Engine::BoxCollider& collider = ecsSystem->GetComponent<Engine::BoxCollider>(entity);
        for(auto collision : collider.collisions)
        {
            Engine::Entity other = collision.first.other;
            //If is a bullet and was not shot from an enemy
            if(ecsSystem->HasComponent<Bullet>(other) && !ecsSystem->HasComponent<EnemyBehaviour>(ecsSystem->GetComponent<Bullet>(other).spawner))
            {
                switch (behaviour.behaviour)
                {
                    default:
                    case EnemyBehaviour::Hubertus:
                        HandleDamageHubertus(entity, other);
                        break;
                    case EnemyBehaviour::KindredSpirit:
                        HandleDamageKindredSpirit(entity, other);
                        break;
                }
            }
        }
    }
}

void EnemyBehaviourSystem::UpdateHubertus(Engine::Entity entity, float deltaTime)
{
    EnemyBehaviour &behaviour = ecsSystem->GetComponent<EnemyBehaviour>(entity);
    Engine::Transform &transform = ecsSystem->GetComponent<Engine::Transform>(entity);
    if(behaviour.isMoving)
        MoveEnemy(behaviour, transform, deltaTime);

    std::random_device rd;
    std::mt19937 gen(rd());
    if(behaviour.shootTimer <= 0)
    {
        ECSHelper::SpawnBullet(entity, transform.GetGlobalTranslation(), glm::vec3(behaviour.movement, 0));
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
}

void EnemyBehaviourSystem::HandleDamageHubertus(Engine::Entity entity, Engine::Entity other)
{
    //Bullet is already destroying itself, so no need to do it here
    Health& health = ecsSystem->GetComponent<Health>(entity);
    health.health--;
    if(health.health <= 0)
    {
        RemoveEntityWithChildren(entity);
        ECSHelper::SpawnLoot(ecsSystem->GetComponent<Engine::Transform>(entity).GetGlobalTranslation(), EnemyBehaviour::scores[EnemyBehaviour::Hubertus]);
    }
}

void EnemyBehaviourSystem::UpdateKindredSpirit(Engine::Entity entity, float deltaTime)
{
    EnemyBehaviour &behaviour = ecsSystem->GetComponent<EnemyBehaviour>(entity);
    Engine::Transform &transform = ecsSystem->GetComponent<Engine::Transform>(entity);

    if(behaviour.enemyExtra.kindredSpirit.isMainEntity)
    {
        MoveEnemy(behaviour, transform, deltaTime);
    }
    else
    {
        EnemyBehaviour &mainEnemy = ecsSystem->GetComponent<EnemyBehaviour>(behaviour.enemyExtra.kindredSpirit.other);
        Engine::Transform &mainTransform = ecsSystem->GetComponent<Engine::Transform>(behaviour.enemyExtra.kindredSpirit.other);

        transform.SetTranslation(mainTransform.GetTranslation() * -1.0f);
        transform.SetRotation(mainTransform.GetRotation() * glm::quat(glm::vec3(0, 0, glm::radians((180.0f)))));
        behaviour.isMoving = mainEnemy.isMoving;
    }
}

void EnemyBehaviourSystem::HandleDamageKindredSpirit(Engine::Entity entity, Engine::Entity other)
{
    EnemyBehaviour& behaviour = ecsSystem->GetComponent<EnemyBehaviour>(entity);
    EnemyBehaviour& otherSpiritBehaviour = ecsSystem->GetComponent<EnemyBehaviour>(behaviour.enemyExtra.kindredSpirit.other);

    behaviour.enemyExtra.kindredSpirit.timeWhenLastHit = std::chrono::system_clock::now();
    std::chrono::duration<float> timeDifference = (behaviour.enemyExtra.kindredSpirit.timeWhenLastHit - otherSpiritBehaviour.enemyExtra.kindredSpirit.timeWhenLastHit);

    if(abs(timeDifference.count()) <= KindredSpiritExtra::maxTimeDifference)
    {
        //Bullet is already destroying itself, so no need to do it here
        //Get the health of the main kindred spirit
        Health &health = behaviour.enemyExtra.kindredSpirit.isMainEntity ? ecsSystem->GetComponent<Health>(entity) : ecsSystem->GetComponent<Health>(behaviour.enemyExtra.kindredSpirit.other);

        health.health--;
        if (health.health <= 0)
        {
            RemoveEntityWithChildren(entity);
            ECSHelper::SpawnLoot(ecsSystem->GetComponent<Engine::Transform>(entity).GetGlobalTranslation(),EnemyBehaviour::scores[EnemyBehaviour::KindredSpirit]);

            RemoveEntityWithChildren(behaviour.enemyExtra.kindredSpirit.other);
            ECSHelper::SpawnLoot(ecsSystem->GetComponent<Engine::Transform>(behaviour.enemyExtra.kindredSpirit.other).GetGlobalTranslation(),EnemyBehaviour::scores[EnemyBehaviour::KindredSpirit]);
        }
    }
}

void EnemyBehaviourSystem::MoveEnemy(EnemyBehaviour& behaviour, Engine::Transform& transform, float deltaTime)
{
    transform.AddTranslation(glm::vec3(behaviour.movement * behaviour.movementSpeed * deltaTime, 0));
    if(glm::length(behaviour.targetPos - glm::vec2(transform.GetGlobalTranslation())) < behaviour.movementSpeed * deltaTime * 2)
    {
        std::list<std::pair<int, int>>& list = graph[behaviour.targetNode];
        auto iter = list.end();
        int i = 0;
        do
        {
            int index = rand() % list.size();
            iter = list.begin();
            std::advance(iter, index);
            i++;
        } while(i < 5 && *iter == behaviour.oldTargetNode);
        behaviour.oldTargetNode = behaviour.targetNode;
        behaviour.targetNode = *iter;
        behaviour.targetPos = ToGlobal(behaviour.targetNode);
        behaviour.movement = glm::normalize(ToGlobal(behaviour.targetNode) -  ToGlobal(behaviour.oldTargetNode));
        transform.SetTranslation(glm::vec3(ToGlobal(behaviour.oldTargetNode), transform.GetTranslation().z));
        transform.SetRotation(glm::quat(glm::vec3(glm::radians(90.0f), 0, glm::atan(behaviour.movement.y, behaviour.movement.x))));
    }
}

/// Finds a node by raytracing into a given direction from a given point
/// \param startx
/// \param starty
/// \param dirx
/// \param diry
/// \return
std::pair<int, int> EnemyBehaviourSystem::FindNode(int startx, int starty, int dirx, int diry)
{
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
    return std::make_pair(-1, -1);
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

/// This needs to be called before the first call to Update
/// \param wallMap a bool map of the dungeon, where true marks a wall, a false marks free space
void EnemyBehaviourSystem::Initialize(std::vector<std::vector<bool>>& wallMap)
{
    this->wallMap = wallMap;
    dungeonSize = std::make_pair(wallMap.size(), wallMap[0].size());
    originOffset = glm::vec2(dungeonSize.first, dungeonSize.second) / -2.0f + 0.5f;
    originOffset.y *= -1.0f;
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

/// Transforms the given point, which is in dungeon coordinates, into global coordinates
/// \param dungeonPos
/// \return
glm::vec2 EnemyBehaviourSystem::ToGlobal(glm::vec2 dungeonPos)
{
    return glm::vec2(originOffset.x + dungeonPos.x, originOffset.y - dungeonPos.y);
}


glm::vec2 EnemyBehaviourSystem::ToGlobal(std::pair<int, int> dungeonPos)
{
    return glm::vec2(originOffset.x + dungeonPos.first, originOffset.y - dungeonPos.second);
}
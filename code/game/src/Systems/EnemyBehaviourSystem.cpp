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

extern std::shared_ptr<PlayerControllerSystem> playerControllerSystem;

EnemyBehaviourSystem::EnemyBehaviourSystem()
{
    srand(std::chrono::system_clock::now().time_since_epoch().count());

    enemyScoreDecrease = Defines::Float("Enemy_ScoreDecrease");

    idleDurationRanges[EnemyBehaviour::Hubertus].first = Defines::Float("Hubertus_IdleDuration_Min");
    idleDurationRanges[EnemyBehaviour::Hubertus].second = Defines::Float("Hubertus_IdleDuration_Max");
    walkDurationRanges[EnemyBehaviour::Hubertus].first = Defines::Float("Hubertus_WalkDuration_Min");
    walkDurationRanges[EnemyBehaviour::Hubertus].second = Defines::Float("Hubertus_WalkDuration_Max");
    shootIntervalRanges[EnemyBehaviour::Hubertus].first = Defines::Float("Hubertus_ShootInterval_Min");
    shootIntervalRanges[EnemyBehaviour::Hubertus].second = Defines::Float("Hubertus_ShootInterval_Max");

    idleDurationRanges[EnemyBehaviour::Assi].first = Defines::Float("Assi_IdleDuration_Min");
    idleDurationRanges[EnemyBehaviour::Assi].second = Defines::Float("Assi_IdleDuration_Max");

    KindredSpiritExtra::maxTimeDifference = Defines::Float("KindredSpirit_TimeTolerance");
    KindredSpiritExtra::colours.push({1.00f, 0.68f, 0.21f, 1.0f});
    KindredSpiritExtra::colours.push({0.27f, 0.17f, 1.00f, 1.0f});
    KindredSpiritExtra::colours.push({0.88f, 0.13f, 0.29f, 1.0f});
    KindredSpiritExtra::colours.push({0.28f, 1.00f, 0.98f, 1.0f});
    KindredSpiritExtra::colours.push({0.63f, 0.28f, 1.00f, 1.0f});
    KindredSpiritExtra::colours.push({0.58f, 1.00f, 0.28f, 1.0f});
    KindredSpiritExtra::colours.push({1.00f, 1.00f, 1.00f, 1.0f});
    KindredSpiritExtra::colours.push({0.40f, 0.40f, 0.40f, 1.0f});

    AssiExtra::stunnedTime = Defines::Float("Assi_StunnedTime");
}

void EnemyBehaviourSystem::EntityAdded(Engine::Entity entity)
{
    EnemyBehaviour &behaviour = ecsSystem->GetComponent<EnemyBehaviour>(entity);
    Engine::Transform& transform = ecsSystem->GetComponent<Engine::Transform>(entity);

    transform.SetTranslation(glm::vec3(ToGlobal(behaviour.startPos), 0));

    if(behaviour.behaviour == EnemyBehaviour::KindredSpirit && behaviour.enemyExtra.kindredSpirit.isMainEntity == false)
    {
            transform.SetTranslation(glm::vec3(ToGlobal(behaviour.startPos) * -1.0f, 0));
            return;
    }

    std::vector<std::pair<int,int>> targetNodes = FindNodes(behaviour.startPos.first, behaviour.startPos.second);
    std::pair<int, int> target = targetNodes[rand() % targetNodes.size()];
    behaviour.oldTargetNode = behaviour.startPos;
    behaviour.targetNode = target;
    behaviour.targetPos = ToGlobal(behaviour.targetNode);
    behaviour.movement = glm::normalize(ToGlobal(behaviour.targetNode) -  ToGlobal(behaviour.oldTargetNode));
    transform.SetRotation(glm::quat(glm::vec3(glm::radians(90.0f), 0, glm::atan(behaviour.movement.y, behaviour.movement.x))));
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> distr(shootIntervalRanges[behaviour.behaviour].first, shootIntervalRanges[behaviour.behaviour].second);
    behaviour.shootTimer = distr(gen);
    distr = std::uniform_real_distribution<>(walkDurationRanges[behaviour.behaviour].first, walkDurationRanges[behaviour.behaviour].second);
    behaviour.idleTimer = distr(gen);
    behaviour.isMoving = true;
}

void EnemyBehaviourSystem::EntityRemoved(Engine::Entity entity)
{
    EnemyBehaviour& enemy = ecsSystem->GetComponent<EnemyBehaviour>(entity);
    if(enemy.behaviour == EnemyBehaviour::KindredSpirit && enemy.enemyExtra.kindredSpirit.isMainEntity)
    {
        glm::vec4 colour = Engine::GetComponentsInChildren<Engine::MeshRenderer>(entity)[0]->primitiveData[0].material.baseColorFactor;
        KindredSpiritExtra::colours.push(colour);
    }
}

void EnemyBehaviourSystem::Update(float deltaTime)
{
    for (Engine::Entity entity: entities)
    {
        EnemyBehaviour &behaviour = ecsSystem->GetComponent<EnemyBehaviour>(entity);
        if(!behaviour.isActive) continue;
        switch (behaviour.behaviour)
        {
            default:
            case EnemyBehaviour::Hubertus:
                UpdateHubertus(entity, deltaTime);
                break;
            case EnemyBehaviour::KindredSpirit:
                UpdateKindredSpirit(entity, deltaTime);
                break;
            case EnemyBehaviour::Assi:
                UpdateAssi(entity, deltaTime);
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
                    case EnemyBehaviour::Assi:
                        HandleDamageAssi(entity, other);
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

    std::random_device rd;
    std::mt19937 gen(rd());
    if(behaviour.shootTimer <= 0)
    {
        ECSHelper::SpawnBullet(entity, transform.GetGlobalTranslation(), glm::vec3(behaviour.movement, 0), behaviour.bulletSpeed);
        std::uniform_real_distribution<> distr(shootIntervalRanges[EnemyBehaviour::Hubertus].first, shootIntervalRanges[EnemyBehaviour::Hubertus].second);
        behaviour.shootTimer = distr(gen);
    }
    if(behaviour.idleTimer <= 0)
    {
        std::uniform_real_distribution<> distr;
        if(behaviour.isMoving)
            distr = std::uniform_real_distribution<>(idleDurationRanges[EnemyBehaviour::Hubertus].first, idleDurationRanges[EnemyBehaviour::Hubertus].second);
        else
            distr = std::uniform_real_distribution<>(walkDurationRanges[EnemyBehaviour::Hubertus].first, walkDurationRanges[EnemyBehaviour::Hubertus].second);
        behaviour.isMoving = !behaviour.isMoving;
        behaviour.idleTimer = distr(gen);
    }
    behaviour.idleTimer -= deltaTime;
    behaviour.shootTimer -= deltaTime;

    if(behaviour.isMoving)
        MoveEnemyNormal(behaviour, transform, deltaTime);
}

void EnemyBehaviourSystem::HandleDamageHubertus(Engine::Entity entity, Engine::Entity other)
{
    //Bullet is already destroying itself, so no need to do it here
    Health& health = ecsSystem->GetComponent<Health>(entity);
    EnemyBehaviour& behaviour = ecsSystem->GetComponent<EnemyBehaviour>(entity);
    health.health--;
    if(health.health <= 0)
    {
        RemoveEntityWithChildren(entity);
        float timeAlive = Engine::Systems::timeManager->GetTimeSinceStartup() - behaviour.spawnTime;
        int score = EnemyBehaviour::scores[EnemyBehaviour::Hubertus] - (int)(timeAlive * enemyScoreDecrease);
        score = score < 1 ? 1 : score;
        ECSHelper::SpawnLoot(ecsSystem->GetComponent<Engine::Transform>(entity).GetGlobalTranslation(), score);
    }
}

void EnemyBehaviourSystem::UpdateKindredSpirit(Engine::Entity entity, float deltaTime)
{
    EnemyBehaviour &behaviour = ecsSystem->GetComponent<EnemyBehaviour>(entity);
    Engine::Transform &transform = ecsSystem->GetComponent<Engine::Transform>(entity);

    if(behaviour.enemyExtra.kindredSpirit.isMainEntity)
    {
        MoveEnemyNormal(behaviour, transform, deltaTime);
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

void EnemyBehaviourSystem::UpdateAssi(Engine::Entity entity, float deltaTime)
{
    EnemyBehaviour &behaviour = ecsSystem->GetComponent<EnemyBehaviour>(entity);
    Engine::Transform &transform = ecsSystem->GetComponent<Engine::Transform>(entity);

    if(behaviour.enemyExtra.assi.stunnedTimer > 0.0f)
    {
        behaviour.enemyExtra.assi.stunnedTimer -= deltaTime;
        return;
    }

    if(behaviour.isMoving)
    {
        Engine::Entity player = FindPlayerInSight(entity, INT_MAX);
        if(player != Engine::Entity::INVALID_ENTITY_ID)
        {
            behaviour.enemyExtra.assi.isPlayerInSight = true;
            glm::vec3 direction = ecsSystem->GetComponent<Engine::Transform>(player).GetGlobalTranslation() - transform.GetGlobalTranslation();
            MoveAssi(behaviour, transform, direction, deltaTime);
        }
        else if (behaviour.enemyExtra.assi.isPlayerInSight)
        {
            behaviour.enemyExtra.assi.isPlayerInSight = false;

            std::pair<int, int> pos = ToDungeon(glm::round(transform.GetGlobalTranslation()));
            std::vector<std::pair<int,int>> targetNodes = FindNodes(pos.first, pos.second);
            std::pair<int, int> target = targetNodes[rand() % targetNodes.size()];

            behaviour.targetNode = target;
            behaviour.targetPos = ToGlobal(behaviour.targetNode);
            behaviour.movement = ToGlobal(behaviour.targetNode) - glm::vec2(glm::round(transform.GetGlobalTranslation()));
            if(behaviour.movement != glm::vec2(0))
                behaviour.movement = glm::normalize(behaviour.movement);
            transform.SetRotation(glm::quat(glm::vec3(glm::radians(90.0f), 0, glm::atan(behaviour.movement.y, behaviour.movement.x))));
        }
        else
            MoveEnemyNormal(behaviour, transform, deltaTime);
    }

    if(behaviour.idleTimer <= 0)
    {
        std::uniform_real_distribution<> distr;
        if(behaviour.isMoving)
        {
            std::random_device rd;
            std::mt19937 gen(rd());
            if(behaviour.enemyExtra.assi.isPlayerInSight)
                distr = std::uniform_real_distribution<>(idleDurationRanges[EnemyBehaviour::Assi].first, idleDurationRanges[EnemyBehaviour::Assi].first);
            else
                distr = std::uniform_real_distribution<>(idleDurationRanges[EnemyBehaviour::Assi].first, idleDurationRanges[EnemyBehaviour::Assi].second);
            behaviour.idleTimer = distr(gen);

            auto pos = transform.GetGlobalTranslation();
            std::cout << pos.x << " " << pos.y << " " << pos.z << std::endl;
            transform.SetTranslation(glm::round(transform.GetTranslation()));
        }
        else
        {
            behaviour.idleTimer = 1 / behaviour.speed; //How long it takes to move by one unit
        }
        behaviour.isMoving = !behaviour.isMoving;
    }
    behaviour.idleTimer -= deltaTime;
}

void EnemyBehaviourSystem::HandleDamageAssi(Engine::Entity entity, Engine::Entity other)
{
    //Bullet is already destroying itself, so no need to do it here
    EnemyBehaviour& behaviour = ecsSystem->GetComponent<EnemyBehaviour>(entity);

    Engine::Transform& transform = ecsSystem->GetComponent<Engine::Transform>(entity);
    Engine::Transform& otherTransform = ecsSystem->GetComponent<Engine::Transform>(other);
    glm::vec2 dir = otherTransform.GetGlobalTranslation() - transform.GetGlobalTranslation();
    dir = Miscellaneous::RoundTo8Directions(glm::vec3(dir, 0));
    glm::quat rot = glm::normalize(glm::quat(glm::vec3(glm::radians(90.0f), 0, glm::atan(dir.y, dir.x))));
    if(behaviour.enemyExtra.assi.stunnedTimer <= 0 || rot == transform.GetRotation())
    {
        behaviour.enemyExtra.assi.stunnedTimer = AssiExtra::stunnedTime;
        transform.SetRotation(rot);
        return;
    }

    Health& health = ecsSystem->GetComponent<Health>(entity);
    health.health--;
    if(health.health <= 0)
    {
        RemoveEntityWithChildren(entity);
        float timeAlive = Engine::Systems::timeManager->GetTimeSinceStartup() - behaviour.spawnTime;
        int score = EnemyBehaviour::scores[EnemyBehaviour::Assi] - (int)(timeAlive * enemyScoreDecrease);
        score = score < 1 ? 1 : score;
        ECSHelper::SpawnLoot(ecsSystem->GetComponent<Engine::Transform>(entity).GetGlobalTranslation(), score);
    }
}


void EnemyBehaviourSystem::MoveEnemyNormal(EnemyBehaviour& behaviour, Engine::Transform& transform, float deltaTime)
{
    transform.AddTranslation(glm::vec3(behaviour.movement * behaviour.speed * deltaTime, 0));
    if(glm::length(behaviour.targetPos - glm::vec2(transform.GetGlobalTranslation())) < behaviour.speed * deltaTime * 2)
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

void EnemyBehaviourSystem::MoveAssi(EnemyBehaviour &behaviour, Engine::Transform &transform, glm::vec3 direction, float deltaTime)
{
    transform.AddTranslation(glm::vec3(behaviour.movement * behaviour.speed * deltaTime, 0));

    if(direction != glm::vec3(0))
        direction = glm::normalize(direction);

    behaviour.movement = direction;
    transform.SetRotation(glm::quat(glm::vec3(glm::radians(90.0f), 0, glm::atan(behaviour.movement.y, behaviour.movement.x))));
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
    ChangeWallMap(wallMap);
}

void EnemyBehaviourSystem::ChangeWallMap(std::vector<std::vector<bool>> &wallMap)
{
    this->wallMap = wallMap;
    graph.clear();
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

std::pair<int, int> EnemyBehaviourSystem::ToDungeon(glm::vec3 globalPos)
{
    return {glm::round(globalPos.x - originOffset.x), glm::round(originOffset.y - globalPos.y)};
}

Engine::Entity EnemyBehaviourSystem::FindPlayerInSight(Engine::Entity enemy, int maxDistance)
{
    Engine::Transform& enemyTransform = ecsSystem->GetComponent<Engine::Transform>(enemy);
    glm::vec3 enemyPos = enemyTransform.GetGlobalTranslation();
    std::pair<int, int> enemyPosDungeon = ToDungeon(enemyPos);
    for(Engine::Entity player : playerControllerSystem->entities)
    {
        glm::vec3 distanceToPlayer = ecsSystem->GetComponent<Engine::Transform>(player).GetGlobalTranslation() - enemyPos;
        glm::vec3 direction = Miscellaneous::RoundTo4Directions(glm::normalize(distanceToPlayer));

        if(length(direction - glm::normalize(distanceToPlayer)) > 0.1f) continue;

        std::pair<int, int> wall = FindWall(enemyPosDungeon.first, enemyPosDungeon.second, (int)direction.x, (int)direction.y);
        glm::vec3 distanceToWall = glm::vec3(ToGlobal(wall), 0) - enemyPos;
        if(glm::length(distanceToPlayer) < maxDistance && glm::length(distanceToPlayer) < glm::length(distanceToWall))
        {
            return player;
        }
    }

    return Engine::Entity::INVALID_ENTITY_ID;
}

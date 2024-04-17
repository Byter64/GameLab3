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

    idleDurationRanges[EnemyBehaviour::Assi].first = Defines::Float("Assi_IdleDuration_Min");
    idleDurationRanges[EnemyBehaviour::Assi].second = Defines::Float("Assi_IdleDuration_Max");

    AssiExtra::stunnedTime = Defines::Float("Assi_StunnedTime");
    DukeExtra::detectionRadius = Defines::Float("Duke_DetectionRadius");
    DukeExtra::minPlayerDistance = Defines::Float("Duke_MinPlayerDistance");
    DukeExtra::prefPlayerDistance = Defines::Float("Duke_PrefPlayerDistance");

    generator.setHeuristic(AStar::Heuristic::euclidean);
    generator.setDiagonalMovement(false);
}

void EnemyBehaviourSystem::EntityAdded(Engine::Entity entity)
{
    EnemyBehaviour &behaviour = ecsSystem->GetComponent<EnemyBehaviour>(entity);
    Engine::Transform& transform = ecsSystem->GetComponent<Engine::Transform>(entity);

    transform.SetTranslation(glm::vec3(ToGlobal(behaviour.startPos), 0));

    std::vector<std::pair<int,int>> targetNodes = FindNodes(behaviour.startPos.first, behaviour.startPos.second);
    std::pair<int, int> target = targetNodes[rand() % targetNodes.size()];
    SetTarget(behaviour, transform, behaviour.startPos, target, true);

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
    Systems::dungeonSystem->OnEnemyDestroyed(entity);
}

void EnemyBehaviourSystem::Update(float deltaTime)
{
    for (Engine::Entity entity: entities)
    {
        EnemyBehaviour &behaviour = ecsSystem->GetComponent<EnemyBehaviour>(entity);
        switch (behaviour.behaviour)
        {
            default:
            case EnemyBehaviour::Cuball:        UpdateCuball(entity, deltaTime); break;
            case EnemyBehaviour::Duke:          UpdateDuke(entity, deltaTime); break;
        }
        Engine::BoxCollider& collider = ecsSystem->GetComponent<Engine::BoxCollider>(entity);
        for(auto& collision : collider.collisions)
        {
            Engine::Entity other = collision.other;
            //If is a bullet and was not shot from an enemy
            if(ecsSystem->HasComponent<Bullet>(other) && !ecsSystem->HasComponent<EnemyBehaviour>(ecsSystem->GetComponent<Bullet>(other).spawner))
            {
                switch (behaviour.behaviour)
                {
                    default:
                    case EnemyBehaviour::Cuball:        HandleDamageCuball(entity, other); break;
                    case EnemyBehaviour::Duke:          HandleDamageDuke(entity, other); break;
                }
            }
        }
    }
}

void EnemyBehaviourSystem::UpdateCuball(Engine::Entity entity, float deltaTime)
{
    EnemyBehaviour &behaviour = ecsSystem->GetComponent<EnemyBehaviour>(entity);
    Engine::Transform &transform = ecsSystem->GetComponent<Engine::Transform>(entity);
    std::pair<int, int> dungeonSize = dungeonSystem->GetDungeonSize();

    if(!ecsSystem->HasComponent<Engine::Animator>(entity))
    {
        behaviour.isActive = true;
        ecsSystem->GetComponent<Engine::BoxCollider>(entity).layer = (int)CollisionLayer::Enemy;
    }
    if(!ecsSystem->HasComponent<Engine::Animator>(entity) && behaviour.enemyExtra.cuball.phase == CuballExtra::MoveToNewPosition)
    {
        behaviour.isActive = false;
        ecsSystem->GetComponent<Engine::BoxCollider>(entity).layer = (int)CollisionLayer::Ignore;

        std::pair<int, int> pos{-1, -1};
        while(IsWall(pos))
        {
            std::pair<int, int> dungeonPos = ToDungeon(transform.GetTranslation());
            int maxX = CuballExtra::mirrorDirection.first == 1 ? dungeonSize.first / 2 : dungeonSize.first;
            int maxY = CuballExtra::mirrorDirection.second == 1 ? dungeonSize.second / 2 : dungeonSize.second;
            int offsetX = dungeonPos.first < maxX && CuballExtra::mirrorDirection.first == 1 ? maxX : 0;
            int offsetY = dungeonPos.second < maxY && CuballExtra::mirrorDirection.second == 1 ? maxY : 0;

            pos = {rand() % maxX, rand() % maxY};
            pos.first += offsetX;
            pos.second += offsetY;
        }
        transform.SetTranslation(glm::vec3(ToGlobal(pos), 0));
        Engine::Systems::animationSystem->PlayAnimation(entity, "Cuball_Spawning");
        behaviour.enemyExtra.cuball.phase = CuballExtra::BecomeBall;
    }
    else if(!ecsSystem->HasComponent<Engine::Animator>(entity) && behaviour.enemyExtra.cuball.phase == CuballExtra::BecomeBall)
    {
        behaviour.isActive = false;
        ecsSystem->GetComponent<Engine::BoxCollider>(entity).layer = (int)CollisionLayer::Ignore;

        ecsSystem->GetComponent<Engine::MeshRenderer>(ecsSystem->GetEntity(*transform.GetChild(0))).isActive = true;
        ecsSystem->GetComponent<Engine::MeshRenderer>(ecsSystem->GetEntity(*transform.GetChild(1))).isActive = false;
        for(auto& renderer : Engine::GetComponentsInChildren<Engine::MeshRenderer>(ecsSystem->GetEntity(*transform.GetChild(2))))
            renderer->isActive = true;

        behaviour.isActive = false;
        Engine::Systems::animationSystem->PlayAnimation(entity, "Cuball_CubeToBall");
        behaviour.enemyExtra.cuball.phase = CuballExtra::DisableMeshes;
    }
    else if(!ecsSystem->HasComponent<Engine::Animator>(entity) && behaviour.enemyExtra.cuball.phase == CuballExtra::DisableMeshes)
    {
        for(auto& renderer : Engine::GetComponentsInChildren<Engine::MeshRenderer>(ecsSystem->GetEntity(*transform.GetChild(2))))
            renderer->isActive = false;

        std::pair<int, int> dungeonPos = ToDungeon(transform.GetGlobalTranslation());
        std::vector<std::pair<int,int>> targetNodes = FindNodes(dungeonPos.first, dungeonPos.second);
        std::pair<int, int> target = targetNodes[rand() % targetNodes.size()];
        behaviour.oldTargetNode = dungeonPos;
        behaviour.targetNode = target;
        behaviour.targetPos = ToGlobal(behaviour.targetNode);
        behaviour.movement = glm::normalize(ToGlobal(behaviour.targetNode) - ToGlobal(behaviour.oldTargetNode));

        behaviour.enemyExtra.cuball.phase = CuballExtra::Ball;
    }

    if(!behaviour.isActive) return;

    MoveCuball(behaviour, transform, deltaTime);
}

void EnemyBehaviourSystem::HandleDamageCuball(Engine::Entity entity, Engine::Entity other)
{
    //Bullet is already destroying itself, so no need to do it here
    Health& health = ecsSystem->GetComponent<Health>(entity);
    EnemyBehaviour& behaviour = ecsSystem->GetComponent<EnemyBehaviour>(entity);
    CuballExtra& extra = behaviour.enemyExtra.cuball;

    if(!behaviour.isActive) return;

    health.health--;
    if(health.health <= 0)
    {
        if(extra.phase == CuballExtra::Cube)
        {
            behaviour.isActive = false;
            health.health = Defines::Int("Cuball_Ball_Health");
            health.maxHealth = Defines::Int("Cuball_Ball_Health");
            behaviour.bulletSpeed = Defines::Float("Cuball_Ball_BulletSpeed");
            behaviour.speed = Defines::Float("Cuball_Ball_Speed");

            Engine::Transform& transform = ecsSystem->GetComponent<Engine::Transform>(entity);
            Engine::Transform& parent = ecsSystem->GetComponent<Engine::Transform>(extra.rotationParent);
            glm::vec3 pos = transform.GetGlobalTranslation();
            transform.SetParent(nullptr);
            transform.SetRotation(glm::quat({glm::radians(90.0f), 0, 0}));
            transform.SetTranslation(pos);

            ecsSystem->RemoveEntity(extra.rotationParent);
            extra.rotationParent = Engine::Entity::INVALID_ENTITY_ID;

            Engine::Animation& cuballSpawning = Engine::Systems::animationSystem->GetAnimation("Cuball_Spawning");
            Engine::Systems::animationSystem->PlayAnimation(entity, "Cuball_Spawning", false, cuballSpawning.endTime, -1);

            extra.phase = CuballExtra::MoveToNewPosition;
            return;
        }

        RemoveEntityWithChildren(entity);
        float timeAlive = Engine::Systems::timeManager->GetTimeSinceStartup() - behaviour.spawnTime;
        int score = EnemyBehaviour::scores[EnemyBehaviour::Cuball] - (int)(timeAlive * enemyScoreDecrease);
        score = score < 1 ? 1 : score;
        ECSHelper::SpawnLoot(ecsSystem->GetComponent<Engine::Transform>(entity).GetGlobalTranslation(), score);
    }
}

void EnemyBehaviourSystem::UpdateDuke(Engine::Entity entity, float deltaTime)
{
    EnemyBehaviour& behaviour = ecsSystem->GetComponent<EnemyBehaviour>(entity);
    Engine::Transform& transform = ecsSystem->GetComponent<Engine::Transform>(entity);
    DukeExtra& dukeExtra = behaviour.enemyExtra.dukeExtra;

    switch (dukeExtra.bigPhase)
    {
        case DukeExtra::BigPhase::Waiting:
        {
            int player1XPos = -1, player2XPos = -1;
            player1XPos = ToDungeon(ecsSystem->GetComponent<Engine::Transform>(players.first).GetGlobalTranslation()).first;
            if (players.second != Engine::Entity::INVALID_ENTITY_ID)
                player2XPos = ToDungeon(ecsSystem->GetComponent<Engine::Transform>(players.second).GetGlobalTranslation()).first;

            if (player1XPos > DukeExtra::preparingThreshold || player2XPos > DukeExtra::preparingThreshold)
            {
                std::pair<int, int> pos = ToDungeon(transform.GetGlobalTranslation());
                std::pair<int, int> target = DukeExtra::preparationPositions.back();
                DukeExtra::preparationPositions.pop_back();
                behaviour.path = GeneratePath(pos, target);
                behaviour.path.pop_back(); //the last node is the one where the duke is already standing so we remove it
                dukeExtra.bigPhase = DukeExtra::BigPhase::Preparing;

                SetTarget(behaviour, transform, ToDungeon(transform.GetGlobalTranslation()), behaviour.path[behaviour.path.size() - 1], true);
                behaviour.path.pop_back();
            }
            break;
        }
        case DukeExtra::BigPhase::Preparing:
            if(behaviour.targetNode != std::make_pair(-1, -1))
                MoveDuke(behaviour, transform, deltaTime);
            else
            {
                Engine::Entity player = FindClosestPlayer(entity);
                glm::vec3 direction = ecsSystem->GetComponent<Engine::Transform>(player).GetGlobalTranslation() - transform.GetGlobalTranslation();
                direction = Miscellaneous::RoundTo4Directions(glm::normalize(direction));
                transform.SetRotation(glm::quat(glm::vec3(glm::radians(90.0f), 0, glm::atan(direction.y, direction.x))));

                int player1XPos = -1, player2XPos = -1;
                player1XPos = ToDungeon(ecsSystem->GetComponent<Engine::Transform>(players.first).GetGlobalTranslation()).first;
                if (players.second != Engine::Entity::INVALID_ENTITY_ID)
                    player2XPos = ToDungeon(ecsSystem->GetComponent<Engine::Transform>(players.second).GetGlobalTranslation()).first;

                if (player1XPos > DukeExtra::fightingThreshold || player2XPos > DukeExtra::fightingThreshold)
                {
                    dukeExtra.bigPhase = DukeExtra::BigPhase::Fighting;
                }
            }
            break;
        case DukeExtra::BigPhase::Fighting:
            switch (dukeExtra.phase)
            {
                case DukeExtra::Phase::Waiting:
                {
                    Engine::Entity player = FindClosestPlayer(entity);
                    Engine::Transform &playerTransform = ecsSystem->GetComponent<Engine::Transform>(player);
                    if (glm::length(playerTransform.GetGlobalTranslation() - transform.GetGlobalTranslation()) < DukeExtra::detectionRadius)
                    {
                        dukeExtra.target = player;
                        dukeExtra.phase = DukeExtra::Phase::Duelling;
                    }
                    break;
                }
                case DukeExtra::Phase::Duelling:
                {
                    Engine::Transform &playerTransform = ecsSystem->GetComponent<Engine::Transform>(dukeExtra.target);
                    glm::vec3 enemyToPlayer = playerTransform.GetGlobalTranslation() - transform.GetGlobalTranslation();
                    float distance = glm::length(enemyToPlayer);
                    if (distance > DukeExtra::detectionRadius && false)
                    {
                        std::pair<int, int> pos = ToDungeon(transform.GetGlobalTranslation());
                        std::pair<int, int> target = DukeExtra::preparationPositions.back();
                        DukeExtra::preparationPositions.pop_back();
                        behaviour.path = GeneratePath(pos, target);
                        behaviour.path.pop_back(); //the last node is the one where the duke is already standing so we remove it
                        dukeExtra.bigPhase = DukeExtra::BigPhase::Preparing;
                        dukeExtra.phase = DukeExtra::Phase::Waiting;

                        SetTarget(behaviour, transform, ToDungeon(transform.GetGlobalTranslation()), behaviour.path[behaviour.path.size() - 1], true);
                        behaviour.path.pop_back();
                    }
                    else if (distance < DukeExtra::minPlayerDistance)
                        dukeExtra.phase = DukeExtra::Phase::Retreating;
                    else if (behaviour.targetNode == std::make_pair(-1, -1))
                    {
                        glm::vec3 dir = Miscellaneous::RoundTo4Directions(transform.GetGlobalTranslation() - playerTransform.GetGlobalTranslation());
                        std::pair<int, int> targetPosD = ToDungeon(playerTransform.GetGlobalTranslation() + DukeExtra::prefPlayerDistance * dir);
                        std::pair<int, int> dungeonSize = dungeonSystem->GetDungeonSize();

                        while(IsWall(targetPosD))
                        {
                            if(targetPosD.first > dungeonSize.first - 2)
                                targetPosD.first = dungeonSize.first - 2;
                            if(targetPosD.second > dungeonSize.second - 2)
                                targetPosD.second = dungeonSize.second - 2;
                            if(dir.x == 0)
                            {
                                targetPosD.first += 1;
                                if(!IsWall(targetPosD)) break;
                                targetPosD.first -= 2;
                                if(!IsWall(targetPosD)) break;
                                targetPosD.first += 1;
                            }
                            else
                            {
                                targetPosD.second += 1;
                                if(!IsWall(targetPosD)) break;
                                targetPosD.second -= 2;
                                if(!IsWall(targetPosD)) break;
                                targetPosD.second += 1;
                            }
                            targetPosD.first += dir.x;
                            targetPosD.second += dir.y;
                        }

                        behaviour.path = GeneratePath(ToDungeon(transform.GetGlobalTranslation()), targetPosD);
                        behaviour.path.pop_back();
                        if(behaviour.path.empty()) return;
                        SetTarget(behaviour, transform, ToDungeon(transform.GetGlobalTranslation()), behaviour.path[behaviour.path.size() - 1], true);
                        behaviour.path.pop_back();
                    }
                    else
                    {
                        MoveDuke(behaviour, transform, deltaTime);
                    }
                    break;
                }
                case DukeExtra::Phase::Retreating:
                {
                    Engine::Transform &playerTransform = ecsSystem->GetComponent<Engine::Transform>(dukeExtra.target);
                    float distance = glm::length(playerTransform.GetGlobalTranslation() - transform.GetGlobalTranslation());
                    if(distance > DukeExtra::minPlayerDistance)
                        dukeExtra.phase = DukeExtra::Phase::Duelling;
                    else if (behaviour.targetNode == std::make_pair(-1, -1))
                    {
                        glm::vec3 dir = transform.GetGlobalTranslation() - playerTransform.GetGlobalTranslation();
                        std::pair<int, int> targetPosD = ToDungeon(transform.GetGlobalTranslation() + 2.0f * glm::normalize(dir));
                        while(IsWall(targetPosD))
                        {
                            std::pair<int, int> dungeonSize = dungeonSystem->GetDungeonSize();
                            if(targetPosD.first > dungeonSize.first - 2)
                                targetPosD.first = dungeonSize.first - 2;
                            if(targetPosD.second > dungeonSize.second - 2)
                                targetPosD.second = dungeonSize.second - 2;
                            if(dir.x == 0)
                            {
                                targetPosD.first += 1;
                                if(!IsWall(targetPosD)) break;
                                targetPosD.first -= 2;
                                if(!IsWall(targetPosD)) break;
                                targetPosD.first += 1;
                            }
                            else
                            {
                                targetPosD.second += 1;
                                if(!IsWall(targetPosD)) break;
                                targetPosD.second -= 2;
                                if(!IsWall(targetPosD)) break;
                                targetPosD.second += 1;
                            }
                            targetPosD.first += dir.x;
                            targetPosD.second += dir.y;
                        }
                        behaviour.path = GeneratePath(ToDungeon(transform.GetGlobalTranslation()), targetPosD);
                        behaviour.path.pop_back();
                        if(behaviour.path.empty()) return;
                        SetTarget(behaviour, transform, ToDungeon(transform.GetGlobalTranslation()), behaviour.path[behaviour.path.size() - 1], true);
                        behaviour.path.pop_back();
                    }
                    else
                    {
                        MoveDuke(behaviour, transform, deltaTime);
                    }
                    break;
                }
            }
            break;
    }
}

void EnemyBehaviourSystem::HandleDamageDuke(Engine::Entity entity, Engine::Entity other)
{

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

void EnemyBehaviourSystem::MoveCuball(EnemyBehaviour &behaviour, Engine::Transform &transform, float deltaTime)
{
    CuballExtra& extra = behaviour.enemyExtra.cuball;
    std::pair<int, int> targetNode = behaviour.targetNode;
    if(extra.phase == CuballExtra::Cube)
    {
        extra.progress += deltaTime * behaviour.speed;
        float easedProgress = extra.progress * extra.progress * extra.progress;
        if (easedProgress >= 1.0f)
            easedProgress = 1.0f;
        ecsSystem->GetComponent<Engine::Transform>(extra.rotationParent).SetRotation(
                glm::mix(extra.startRotation, extra.targetRotation, easedProgress));

        if (easedProgress >= 1.0f)
        {
            Engine::Transform &parent = ecsSystem->GetComponent<Engine::Transform>(extra.rotationParent);
            glm::quat rot = transform.GetGlobalRotation();
            glm::vec3 pos = transform.GetGlobalTranslation();
            transform.SetParent(nullptr);
            transform.SetRotation(rot);
            transform.SetTranslation(pos);

            if (glm::length(behaviour.targetPos - glm::vec2(transform.GetGlobalTranslation())) < behaviour.speed * deltaTime * 2)
            {
                std::list<std::pair<int, int>> &list = graph[behaviour.targetNode];
                auto iter = list.end();
                int i = 0;
                do
                {
                    int index = rand() % list.size();
                    iter = list.begin();
                    std::advance(iter, index);
                    i++;
                } while (i < 5 && *iter == behaviour.oldTargetNode);
                SetTarget(behaviour, transform, behaviour.targetNode, *iter, true);
            }

            parent.SetTranslation(transform.GetTranslation() + glm::vec3(behaviour.movement * 0.5f, -0.5f));
            parent.SetRotation(glm::identity<glm::quat>());
            transform.SetParent(&parent);
            transform.SetTranslation(-glm::vec3(behaviour.movement * 0.5f, -0.5f));

            extra.startRotation = glm::identity<glm::quat>();
            glm::vec3 euler = {glm::radians(behaviour.movement.y * -90), glm::radians(behaviour.movement.x * 90), 0};
            extra.targetRotation = glm::quat(euler);
            extra.progress = 0;
        }
    }
    else if (extra.phase == CuballExtra::Ball)
    {
        glm::vec3 distance = transform.GetGlobalTranslation();
        MoveEnemyNormal(behaviour, transform, deltaTime, false);
        distance = transform.GetGlobalTranslation() - distance;
        //distance in radiance = (distance * 2 * pi) / circumference
        distance = distance * -2.0f * glm::pi<float>() / glm::pi<float>();
        glm::vec3 axis = glm::normalize(glm::cross(glm::vec3(0, 0, -1), distance));
        glm::quat rotation = glm::quat(axis * glm::length(distance)) * transform.GetRotation();
        transform.SetRotation(rotation);
    }

    if(targetNode != behaviour.targetNode)
    {
        for(std::pair<int, int> node : graph[targetNode])
        {
            glm::vec3 direction = glm::vec3(ToGlobal(node), 0) - transform.GetGlobalTranslation();
            ECSHelper::SpawnBullet(ecsSystem->GetEntity(transform), transform.GetGlobalTranslation(), direction, behaviour.bulletSpeed);
        }
    }
}


void EnemyBehaviourSystem::MoveDuke(EnemyBehaviour &behaviour, Engine::Transform &transform, float deltaTime)
{
    float speed = behaviour.enemyExtra.dukeExtra.bigPhase == DukeExtra::BigPhase::Preparing ? 1 : behaviour.speed;
    glm::vec3 movement = glm::vec3(behaviour.movement * speed * deltaTime, 0);
    transform.AddTranslation(movement);

    bool isCloseToTarget = glm::length(behaviour.targetPos - glm::vec2(transform.GetGlobalTranslation())) < speed * deltaTime * 2;
    if(isCloseToTarget)
    {
        if(behaviour.path.empty())
        {
            behaviour.targetPos = {-1 ,-1};
            behaviour.oldTargetNode = {-1 ,-1};
            behaviour.targetNode = {-1 ,-1};
            transform.AddTranslation(-movement);
            return;
        }

        SetTarget(behaviour, transform, behaviour.targetNode, behaviour.path[behaviour.path.size() - 1], true);
        behaviour.path.pop_back();
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
    std::pair<int, int> dungeonSize = dungeonSystem->GetDungeonSize();
    auto const& wallMap = dungeonSystem->GetWallMap();

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
    std::pair<int, int> dungeonSize = dungeonSystem->GetDungeonSize();
    auto const& wallMap = dungeonSystem->GetWallMap();

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
    Engine::Entity entity = dungeonSystem->GetDungeonEntity();
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

        std::pair<int, int> wall = FindWall(enemyPosDungeon.first, enemyPosDungeon.second, (int)direction.x, (int)-direction.y);
        glm::vec3 distanceToWall = glm::vec3(ToGlobal(wall), 0) - enemyPos;
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

void EnemyBehaviourSystem::Kill(Engine::Entity entity)
{
    EnemyBehaviour& behaviour = ecsSystem->GetComponent<EnemyBehaviour>(entity);

    RemoveEntityWithChildren(entity);
    float timeAlive = Engine::Systems::timeManager->GetTimeSinceStartup() - behaviour.spawnTime;
    int score = EnemyBehaviour::scores[EnemyBehaviour::Hubertus] - (int)(timeAlive * EnemyBehaviour::scoreDecrease);
    score = score < 1 ? 1 : score;
    ECSHelper::SpawnLoot(ecsSystem->GetComponent<Engine::Transform>(entity).GetGlobalTranslation(), score);
}

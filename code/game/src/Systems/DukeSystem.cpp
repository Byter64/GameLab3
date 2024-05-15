#include "Systems/DukeSystem.h"
#include "GameDefines.h"
#include "ECSExtension.h"

extern std::pair<Engine::Entity, Engine::Entity> players;

DukeSystem::DukeSystem()
{
    Duke::minDistanceToPlayer = Defines::Float("Duke_MinDistanceToPlayer");
    Duke::minDistanceToPlayerSpawning = Defines::Float("Duke_MinDistanceToPlayerSpawning");
    Duke::maxDistanceToPlayerAttacking = Defines::Float("Duke_MaxDistanceToPlayerAttacking");
    Duke::minWalkDistance = Defines::Float("Duke_MinWalkDistance");
    Duke::maxWalkDistance = Defines::Float("Duke_MaxWalkDistance");
    Duke::teleportTime = Defines::Float("Duke_TeleportTime");
    Duke::spawnTime = Defines::Float("Duke_SpawnTime");
}

void DukeSystem::EntityAdded(Engine::Entity entity)
{
    Duke &duke = ecsSystem->GetComponent<Duke>(entity);
    Engine::Transform& transform = ecsSystem->GetComponent<Engine::Transform>(entity);

    transform.SetTranslation(glm::vec3(Systems::dungeonSystem->ToGlobal(duke.startPos), 0));

    duke.movement.oldTargetNode = duke.startPos;
    duke.movement.currentPos = Systems::dungeonSystem->ToGlobal(duke.startPos);

    duke.movement.targetNode = duke.startPos;
    duke.movement.targetPos = duke.movement.currentPos;
    duke.movement.direction = {0, -1};
}

void DukeSystem::EntityRemoved(Engine::Entity entity)
{

}

void DukeSystem::Update(Engine::Entity entity, float deltaTime)
{
    EnemyBehaviour& behaviour = ecsSystem->GetComponent<EnemyBehaviour>(entity);
    Duke& duke = ecsSystem->GetComponent<Duke>(entity);
    Engine::Transform& transform = ecsSystem->GetComponent<Engine::Transform>(entity);

    if(!behaviour.isActive) return;

    switch (duke.phase)
    {
        case Duke::PhaseStart_TeleportStart:
            duke.timer -= deltaTime;
            if(duke.timer <= 0)
            {
                duke.timer = Duke::teleportTime / 2;
                duke.spawnCounter++;

                if(duke.spawnCounter <= Duke::spawnCount[duke.spawnedType])
                {
                    duke.phase = Duke::PhaseStart_TeleportEnd;
                    auto newStartCondition = [&](glm::vec3 startGlobalPos)
                    {
                        return glm::length(startGlobalPos - ecsSystem->GetComponent<Engine::Transform>(players.first).GetGlobalTranslation()) >= Duke::minDistanceToPlayerSpawning &&
                               (players.second == Engine::Entity::INVALID_ENTITY_ID ||
                                glm::length(startGlobalPos - ecsSystem->GetComponent<Engine::Transform>(players.second).GetGlobalTranslation()) >= Duke::minDistanceToPlayerSpawning);
                    };
                    FindNewPosition(duke.movement, newStartCondition);
                }
                else
                {
                    duke.phase = Duke::Tp_TeleportEnd;
                    auto newStartCondition = [&](glm::vec3 startGlobalPos) {
                        return glm::length(startGlobalPos - ecsSystem->GetComponent<Engine::Transform>(players.first).GetGlobalTranslation()) >= Duke::minDistanceToPlayer &&
                               (players.second == Engine::Entity::INVALID_ENTITY_ID ||
                                glm::length(startGlobalPos - ecsSystem->GetComponent<Engine::Transform>(players.second).GetGlobalTranslation()) >= Duke::minDistanceToPlayer);
                    };

                    auto newTargetCondition = [&](glm::vec3 targetGlobalPos) {
                        return glm::length(targetGlobalPos - glm::vec3(duke.movement.currentPos, 0)) >= Duke::minWalkDistance;
                    };
                    bool hasWorked = false;
                    while(!hasWorked)
                    {
                        FindNewPosition(duke.movement, newStartCondition);
                        hasWorked = FindNewTargetPosition(duke.movement, newTargetCondition);
                    }
                }
            }
            break;
        case Duke::PhaseStart_TeleportEnd:
            duke.timer -= deltaTime;
            transform.SetTranslation(glm::vec3(duke.movement.currentPos, 0));
            transform.SetRotation(glm::quat(glm::vec3(glm::radians(90.0f), 0, glm::radians(-90.0f))));
            if (duke.timer <= 0)
            {
                duke.phase = Duke::PhaseStart_SpawnEnemy;
                duke.timer = Duke::spawnTime;
                duke.teleportCounter++;
            }
            break;
        case Duke::PhaseStart_SpawnEnemy:
            duke.timer -= deltaTime;
            if (duke.timer <= 0)
            {
                duke.phase = Duke::PhaseStart_TeleportStart;
                duke.timer = Duke::teleportTime / 2;

                switch (duke.spawnedType)
                {
                    case EnemyBehaviour::Hubertus:
                        ECSHelper::SpawnEnemy(ECSHelper::CreateHubertus(duke.movement.oldTargetNode));
                        break;
                    case EnemyBehaviour::KindredSpirit:
                        ECSHelper::SpawnEnemy(ECSHelper::CreateKindredSpirit(duke.movement.oldTargetNode).first);
                        break;
                    case EnemyBehaviour::Assi:
                        ECSHelper::SpawnEnemy(ECSHelper::CreateAssi(duke.movement.oldTargetNode));
                        break;
                    case EnemyBehaviour::Cuball:
                        ECSHelper::SpawnEnemy(ECSHelper::CreateCuball(duke.movement.oldTargetNode));
                        break;
                }
            }
            break;
        case Duke::Tp_TeleportEnd:
        {
            duke.timer -= deltaTime;
            transform.SetTranslation(glm::vec3(duke.movement.currentPos, 0));
            float angle = glm::atan(duke.movement.direction.y, duke.movement.direction.x);
            angle /= glm::radians(90.0f);
            angle = glm::round(angle);
            angle *= glm::radians(90.0f);
            transform.SetRotation(glm::quat(glm::vec3(glm::radians(90.0f), 0, angle)));
            if (duke.timer <= 0)
            {
                duke.phase = Duke::Tp_Move;
                duke.canShoot = true;
                duke.teleportCounter++;
            }
            break;
        }
        case Duke::Tp_Move:
        {
            float oldDistance = glm::length(duke.movement.currentPos - duke.movement.targetPos);
            Systems::enemyBehaviourSystem->MoveStraight(duke.movement, duke.movement.direction, duke.speed * deltaTime);
            float newDistance = glm::length(duke.movement.currentPos - duke.movement.targetPos);
            transform.SetTranslation(glm::vec3(duke.movement.currentPos, 0));

            float normalizedDistance = 1 - glm::length(duke.movement.currentPos - duke.movement.targetPos) / glm::length(Systems::dungeonSystem->ToGlobal(duke.movement.oldTargetNode) - duke.movement.targetPos);

            if(duke.canShoot && normalizedDistance > 0.5f)
            {
                duke.canShoot = false;
                ECSHelper::SpawnBullet(entity, transform.GetGlobalTranslation(), glm::vec3(duke.movement.direction, 0), duke.bulletSpeed);
            }

            if (newDistance > oldDistance)
            {
                duke.timer = Duke::teleportTime / 2;
                duke.phase = Duke::Tp_TeleportStart;
                if(duke.teleportCounter % 20 == 19)
                    duke.phase = Duke::Tp_TeleportTowardsPlayerStart;
            }
        }
            break;
        case Duke::Tp_TeleportStart:
            duke.timer -= deltaTime;
            if(duke.timer <= 0)
            {
                duke.timer = Duke::teleportTime / 2;
                duke.phase = Duke::Tp_TeleportEnd;

                if(duke.teleportCounter % 101 != 100)
                {
                    auto newStartCondition = [&](glm::vec3 startGlobalPos)
                    {
                        return glm::length(startGlobalPos - ecsSystem->GetComponent<Engine::Transform>(players.first).GetGlobalTranslation()) >= Duke::minDistanceToPlayer &&
                               (players.second == Engine::Entity::INVALID_ENTITY_ID ||
                                glm::length(startGlobalPos - ecsSystem->GetComponent<Engine::Transform>(players.second).GetGlobalTranslation()) >= Duke::minDistanceToPlayer);
                    };

                    auto newTargetCondition = [&](glm::vec3 targetGlobalPos)
                    {
                        return glm::length(targetGlobalPos - glm::vec3(duke.movement.currentPos, 0)) >= Duke::minWalkDistance;
                    };
                    bool hasWorked = false;
                    while(!hasWorked)
                    {
                        FindNewPosition(duke.movement, newStartCondition);
                        hasWorked = FindNewTargetPosition(duke.movement, newTargetCondition);
                    }
                }
                else
                {
                    duke.phase = Duke::Sp_TeleportEnd;
                    auto newStartCondition = [&](glm::vec3 startGlobalPos)
                    {
                        return glm::length(startGlobalPos - ecsSystem->GetComponent<Engine::Transform>(players.first).GetGlobalTranslation()) >= Duke::minDistanceToPlayerSpawning &&
                               (players.second == Engine::Entity::INVALID_ENTITY_ID ||
                                glm::length(startGlobalPos - ecsSystem->GetComponent<Engine::Transform>(players.second).GetGlobalTranslation()) >= Duke::minDistanceToPlayerSpawning);
                    };

                    FindNewPosition(duke.movement, newStartCondition);
                }
            }
            break;
        case Duke::Sp_TeleportEnd:
            duke.timer -= deltaTime;
            transform.SetTranslation(glm::vec3(duke.movement.currentPos, 0));
            transform.SetRotation(glm::quat(glm::vec3(glm::radians(90.0f), 0, glm::radians(-90.0f))));
            if (duke.timer <= 0)
            {
                duke.phase = Duke::Sp_SpawnEnemy;
                duke.timer = Duke::spawnTime;
                duke.teleportCounter++;
            }
            break;
        case Duke::Sp_SpawnEnemy:
            duke.timer -= deltaTime;
            if (duke.timer <= 0)
            {
                duke.phase = Duke::Tp_TeleportStart;
                duke.timer = Duke::teleportTime / 2;

                switch (duke.spawnedType)
                {
                    case EnemyBehaviour::Hubertus:
                        ECSHelper::SpawnEnemy(ECSHelper::CreateHubertus(duke.movement.oldTargetNode));
                        break;
                    case EnemyBehaviour::KindredSpirit:
                        ECSHelper::SpawnEnemy(ECSHelper::CreateKindredSpirit(duke.movement.oldTargetNode).first);
                        break;
                    case EnemyBehaviour::Assi:
                        ECSHelper::SpawnEnemy(ECSHelper::CreateAssi(duke.movement.oldTargetNode));
                        break;
                    case EnemyBehaviour::Cuball:
                        ECSHelper::SpawnEnemy(ECSHelper::CreateCuball(duke.movement.oldTargetNode));
                        break;
                }
            }
            break;
        case Duke::Tp_TeleportTowardsPlayerStart:
            duke.timer = Duke::teleportTime / 2;
            duke.phase = Duke::Tp_TeleportEnd;
            Engine::Entity player = Engine::Entity::INVALID_ENTITY_ID;
            auto newStartCondition = [&](glm::vec3 startGlobalPos) {
                return ((player = Systems::enemyBehaviourSystem->FindPlayerInSight(startGlobalPos, Duke::maxDistanceToPlayerAttacking)) == players.first &&
                                glm::length(ecsSystem->GetComponent<Engine::Transform>(players.first).GetGlobalTranslation() - startGlobalPos) >= 1.0f) ||
                       (players.second != Engine::Entity::INVALID_ENTITY_ID &&
                               (player = Systems::enemyBehaviourSystem->FindPlayerInSight(startGlobalPos, Duke::maxDistanceToPlayerAttacking)) == players.second &&
                               glm::length(ecsSystem->GetComponent<Engine::Transform>(players.second).GetGlobalTranslation() - startGlobalPos) >= 1.0f);
            };

            auto newTargetCondition = [&](glm::vec3 targetGlobalPos) {
                glm::vec3 playerPos = ecsSystem->GetComponent<Engine::Transform>(player).GetGlobalTranslation();
                glm::vec3 x1 = Miscellaneous::RoundTo4Directions(targetGlobalPos - glm::vec3(duke.movement.currentPos, 0));
                glm::vec3 x2 = Miscellaneous::RoundTo4Directions(playerPos - glm::vec3(duke.movement.currentPos, 0));

                return x1 == x2;
            };
            bool hasWorked = false;
                    while(!hasWorked)
                    {
                        FindNewPosition(duke.movement, newStartCondition);
                        hasWorked = FindNewTargetPosition(duke.movement, newTargetCondition);
                    }
            break;
    }
}

///
/// \param movement the movement which should be prepared for teleport
/// \param condition The condition, which must be fulfilled by the new position
void DukeSystem::FindNewPosition(Movement &movement, std::function<bool(glm::vec3 startGlobalPos)> condition)
{
    std::pair<int, int> startDungeonPos;
    glm::vec3 startGlobalPos = glm::vec3(Systems::dungeonSystem->ToGlobal(startDungeonPos), 0);
    do
    {
        startDungeonPos = Systems::dungeonSystem->GetRandomFreePos();
        startGlobalPos = glm::vec3(Systems::dungeonSystem->ToGlobal(startDungeonPos), 0);
    } while (!condition(startGlobalPos));

    movement.oldTargetNode = startDungeonPos;
    movement.currentPos = startGlobalPos;
}

/// This is ugly as hell ~The author
/// \param movement
/// \param condition The condition, which must be fulfilled by the new target position
/// \return
bool DukeSystem::FindNewTargetPosition(Movement &movement, std::function<bool(glm::vec3 targetGlobalPos)> condition)
{
    std::pair<int, int> startDungeonPos = movement.oldTargetNode;
    std::pair<int, int> targetDungeonPos;
    glm::vec3 targetGlobalPos;

    std::list<std::pair<int, int>> paths;
    std::pair<int, int> wall = Systems::enemyBehaviourSystem->FindWall(startDungeonPos.first, startDungeonPos.second, 1, 0);
    wall.first -= 1;
    paths.push_back(wall);
    wall = Systems::enemyBehaviourSystem->FindWall(startDungeonPos.first, startDungeonPos.second, -1, 0);
    wall.first += 1;
    paths.push_back(wall);
    wall = Systems::enemyBehaviourSystem->FindWall(startDungeonPos.first, startDungeonPos.second, 0, 1);
    wall.second -= 1;
    paths.push_back(wall);
    wall = Systems::enemyBehaviourSystem->FindWall(startDungeonPos.first, startDungeonPos.second, 0, -1);
    wall.second += 1;
    paths.push_back(wall);

    while(paths.size() > 0)
    {
        int index = rand() % paths.size();
        auto iter = paths.begin();
        std::advance(iter, index);
        glm::vec2 start = glm::vec2(startDungeonPos.first, startDungeonPos.second);
        glm::vec2 target = glm::vec2(iter->first, iter->second);

        if(condition(glm::vec3(Systems::dungeonSystem->ToGlobal(target), 0)))
        {
            targetDungeonPos = *iter;
            float maxPossibleDistance = glm::length(target - start);
            maxPossibleDistance = std::min(maxPossibleDistance, Duke::maxWalkDistance);

            int distance = rand() % (int)(maxPossibleDistance - Duke::minWalkDistance + 1);
            distance += Duke::minWalkDistance;

            std::pair<int, int> dir = {targetDungeonPos.first - startDungeonPos.first, targetDungeonPos.second - startDungeonPos.second};
            dir.first = ((0 < dir.first) - (dir.first < 0)) * distance;
            dir.second = ((0 < dir.second) - (dir.second < 0)) * distance;

            targetDungeonPos = {startDungeonPos.first + dir.first, startDungeonPos.second + dir.second};
            targetGlobalPos = glm::vec3(Systems::dungeonSystem->ToGlobal(targetDungeonPos), 0);

            movement.targetNode = targetDungeonPos;
            movement.targetPos = targetGlobalPos;
            movement.direction = glm::normalize(movement.targetPos - movement.currentPos);

            return true;
        }
        paths.remove(*iter);
    }

    return false;
}


void DukeSystem::HandleDamage(Engine::Entity entity, Engine::Entity other)
{
    //Bullet is already destroying itself, so no need to do it here
    Health& health = ecsSystem->GetComponent<Health>(entity);
    EnemyBehaviour& behaviour = ecsSystem->GetComponent<EnemyBehaviour>(entity);
    Duke& duke = ecsSystem->GetComponent<Duke>(entity);

    if(!behaviour.isActive) return;

    health.health--;
    duke.spawnedType = static_cast<EnemyBehaviour::Type>(4 - health.health);
    duke.phase = Duke::PhaseStart_TeleportStart;
    duke.timer = 0;
    duke.teleportCounter = 0;
    duke.spawnCounter = 0;
    if(health.health <= 0)
    {
        Systems::enemyBehaviourSystem->Kill(entity, EnemyBehaviour::scores[EnemyBehaviour::Duke]);
    }
}

void DukeSystem::Update(float deltaTime)
{
    for (Engine::Entity entity : entities)
    {
        Update(entity, deltaTime);

        for(auto& collision : ecsSystem->GetComponent<Engine::BoxCollider>(entity).collisions)
        {
            Engine::Entity other = collision.other;
            //If is a bullet and was not shot from an enemy
            if(ecsSystem->HasComponent<Bullet>(other) && !ecsSystem->HasComponent<EnemyBehaviour>(ecsSystem->GetComponent<Bullet>(other).spawner))
                HandleDamage(entity, other);
        }
    }
}
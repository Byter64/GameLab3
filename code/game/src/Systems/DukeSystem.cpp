#include "Systems/DukeSystem.h"
#include "GameDefines.h"
#include "ECSExtension.h"

extern std::pair<Engine::Entity, Engine::Entity> players;

DukeSystem::DukeSystem()
{

}

void DukeSystem::EntityAdded(Engine::Entity entity)
{
    Duke &duke = ecsSystem->GetComponent<Duke>(entity);
    Engine::Transform& transform = ecsSystem->GetComponent<Engine::Transform>(entity);

    transform.SetTranslation(glm::vec3(Systems::dungeonSystem->ToGlobal(duke.startPos), 0));

    duke.movement.oldTargetNode = duke.startPos;
    duke.movement.currentPos = Systems::dungeonSystem->ToGlobal(duke.startPos);

    duke.movement.targetNode = {duke.startPos.first + 5, duke.startPos.second};
    duke.movement.targetPos = Systems::dungeonSystem->ToGlobal(duke.startPos) + glm::vec2(5, 0);
    duke.movement.direction = glm::normalize(duke.movement.targetPos - duke.movement.currentPos);
}

void DukeSystem::EntityRemoved(Engine::Entity entity)
{

}

void DukeSystem::Update(Engine::Entity entity, float deltaTime)
{
    EnemyBehaviour& behaviour = ecsSystem->GetComponent<EnemyBehaviour>(entity);
    Duke& duke = ecsSystem->GetComponent<Duke>(entity);
    Engine::Transform& transform = ecsSystem->GetComponent<Engine::Transform>(entity);

    switch (duke.phase)
    {
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
            }
            break;
        }
        case Duke::Tp_Move:
        {
            float oldDistance = glm::length(duke.movement.currentPos - duke.movement.targetPos);
            Systems::enemyBehaviourSystem->MoveStraight(duke.movement, duke.movement.direction, duke.speed * deltaTime);
            float newDistance = glm::length(duke.movement.currentPos - duke.movement.targetPos);
            transform.SetTranslation(glm::vec3(duke.movement.currentPos, 0));
            if (newDistance > oldDistance)
            {
                duke.movement.currentPos = duke.movement.targetPos;
                duke.timer = Duke::teleportTime / 2;
                duke.phase = Duke::Tp_TeleportStart;

                std::pair<int, int> startDungeonPos;
                glm::vec3 startGlobalPos = glm::vec3(Systems::dungeonSystem->ToGlobal(startDungeonPos), 0);
                std::pair<int, int> targetDungeonPos;
                glm::vec3 targetGlobalPos;
                while (true)
                {
                    startDungeonPos = Systems::dungeonSystem->GetRandomFreePos();
                    startGlobalPos = glm::vec3(Systems::dungeonSystem->ToGlobal(startDungeonPos), 0);

                    bool isCloseToPlayers = glm::length(startGlobalPos - ecsSystem->GetComponent<Engine::Transform>(players.first).GetGlobalTranslation()) < Duke::minDistanceToPlayer &&
                                            ( players.second == Engine::Entity::INVALID_ENTITY_ID ||
                                                  glm::length(startGlobalPos - ecsSystem->GetComponent<Engine::Transform>(players.second).GetGlobalTranslation()) < Duke::minDistanceToPlayer);
                    if(isCloseToPlayers)
                        continue;
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

                        if(glm::length(target - start) >= Duke::minWalkDistance)
                        {
                            targetDungeonPos = *iter;
                            float maxPossibleDistance = glm::length(target - start);
                            maxPossibleDistance = std::min(maxPossibleDistance, Duke::maxWalkDistance);

                            int distance = rand() % (int)(maxPossibleDistance - Duke::minWalkDistance + 1);
                            distance += Duke::minWalkDistance;
                            std::cout << "maxPossibleDistance: " << maxPossibleDistance << " distance: " << distance << std::endl;

                            std::pair<int, int> dir = {targetDungeonPos.first - startDungeonPos.first, targetDungeonPos.second - startDungeonPos.second};
                            dir.first = ((0 < dir.first) - (dir.first < 0)) * distance;
                            dir.second = ((0 < dir.second) - (dir.second < 0)) * distance;

                            targetDungeonPos = {startDungeonPos.first + dir.first, startDungeonPos.second + dir.second};
                            targetGlobalPos = glm::vec3(Systems::dungeonSystem->ToGlobal(targetDungeonPos), 0);

                            std::cout << "startDungeonPos: " << startDungeonPos.first << ", " << startDungeonPos.second;
                            std::cout << " targetDungeonPos: " << targetDungeonPos.first << ", " << targetDungeonPos.second << " Is a wall: " << Systems::dungeonSystem->IsWall(targetDungeonPos) << std::endl;

                            goto NachBeidenWhiles;
                        }
                        paths.remove(*iter);
                    }

                }

                NachBeidenWhiles:

                duke.movement.oldTargetNode = startDungeonPos;
                duke.movement.currentPos = startGlobalPos;

                duke.movement.targetNode = targetDungeonPos;
                duke.movement.targetPos = targetGlobalPos;
                duke.movement.direction = glm::normalize(duke.movement.targetPos - duke.movement.currentPos);

            }
            break;
        }
        case Duke::Tp_TeleportStart:
            duke.timer -= deltaTime;
            if(duke.timer <= 0)
            {
                duke.timer = Duke::teleportTime / 2;
                duke.phase = Duke::Tp_TeleportEnd;
            }
            break;
    }
}

void DukeSystem::HandleDamage(Engine::Entity entity, Engine::Entity other)
{

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
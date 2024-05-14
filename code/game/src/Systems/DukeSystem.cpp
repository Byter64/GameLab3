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
            duke.timer -= deltaTime;
            transform.SetTranslation(glm::vec3(duke.movement.currentPos, 0));
            if(duke.timer <= 0)
            {
                duke.phase = Duke::Tp_Move;
            }
            break;
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

                std::pair<int, int> targetDungeonPos;
                glm::vec3 targetGlobalPos = glm::vec3(Systems::dungeonSystem->ToGlobal(targetDungeonPos), 0);
                do
                {
                    targetDungeonPos = Systems::dungeonSystem->GetRandomFreePos();
                    targetGlobalPos = glm::vec3(Systems::dungeonSystem->ToGlobal(targetDungeonPos), 0);
                } while (glm::length(targetGlobalPos - ecsSystem->GetComponent<Engine::Transform>(players.first).GetGlobalTranslation()) < Duke::minDistanceToPlayer &&
                        ( players.second == Engine::Entity::INVALID_ENTITY_ID ||
                        glm::length(targetGlobalPos - ecsSystem->GetComponent<Engine::Transform>(players.second).GetGlobalTranslation()) < Duke::minDistanceToPlayer));

                duke.movement.oldTargetNode = targetDungeonPos;
                duke.movement.currentPos = targetGlobalPos;

                duke.movement.targetNode = {targetDungeonPos.first + 5, targetDungeonPos.second};
                duke.movement.targetPos = targetGlobalPos + glm::vec3(5, 0, 0);
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
#include "Systems/DukeSystem.h"
#include "GameDefines.h"
#include "ECSExtension.h"

extern std::pair<Engine::Entity, Engine::Entity> players;

DukeSystem::DukeSystem()
{

    Duke::detectionRadius = Defines::Float("Duke_DetectionRadius");
    Duke::minPlayerDistance = Defines::Float("Duke_MinPlayerDistance");
    Duke::prefPlayerDistance = Defines::Float("Duke_PrefPlayerDistance");
}

void DukeSystem::EntityAdded(Engine::Entity entity)
{
    Duke &duke = ecsSystem->GetComponent<Duke>(entity);
    Engine::Transform& transform = ecsSystem->GetComponent<Engine::Transform>(entity);

    transform.SetTranslation(glm::vec3(Systems::dungeonSystem->ToGlobal(duke.startPos), 0));
}

void DukeSystem::EntityRemoved(Engine::Entity entity)
{

}

void DukeSystem::Update(Engine::Entity entity, float deltaTime)
{
    EnemyBehaviour& behaviour = ecsSystem->GetComponent<EnemyBehaviour>(entity);
    Duke& duke = ecsSystem->GetComponent<Duke>(entity);
    Engine::Transform& transform = ecsSystem->GetComponent<Engine::Transform>(entity);
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
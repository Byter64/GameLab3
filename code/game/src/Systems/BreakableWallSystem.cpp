#include "Systems/BreakableWallSystem.h"
#include "ECSExtension.h"

extern std::shared_ptr<EnemyBehaviourSystem> enemyBehaviourSystem;

void BreakableWallSystem::EntityAdded(Engine::Entity entity)
{

}

void BreakableWallSystem::EntityRemoved(Engine::Entity entity)
{

}

void BreakableWallSystem::Update()
{
    for(Engine::Entity entity : entities)
    {
        Engine::BoxCollider& collider = ecsSystem->GetComponent<Engine::BoxCollider>(entity);
        Health& health = ecsSystem->GetComponent<Health>(entity);
        int count = std::count_if(collider.collisions.begin(), collider.collisions.end(),
                                  [](Engine::Collision const &collision){ return collision.state == Engine::Collision::ENTER;});

        health.health -= count;
        if(health.health <= 0)
        {
            Engine::Transform& transform = ecsSystem->GetComponent<Engine::Transform>(entity);
            std::pair<int, int> dungeonPos = EnemyBehaviourSystem::ToDungeon(transform.GetGlobalTranslation());
            enemyBehaviourSystem->ChangeWall(dungeonPos.first, dungeonPos.second, false);
            Engine::RemoveEntityWithChildren(entity);
        }
    }
}

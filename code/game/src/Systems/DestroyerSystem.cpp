#include "Systems/DestroyerSystem.h"
#include "ECSExtension.h"

void DestroyerSystem::EntityAdded(Engine::Entity entity)
{

}

void DestroyerSystem::EntityRemoved(Engine::Entity entity)
{

}

void DestroyerSystem::Update()
{
    for(Engine::Entity entity : entities)
    {
        Engine::BoxCollider& collider = ecsSystem->GetComponent<Engine::BoxCollider>(entity);
        Health& health = ecsSystem->GetComponent<Health>(entity);
        int count = std::count_if(collider.collisions.begin(), collider.collisions.end(),
                                  [](Engine::Collision const &collision){ return collision.state == Engine::Collision::ENTER;});

        health.health -= count;
        if(health.health <= 0)
            Engine::RemoveEntityWithChildren(entity);
    }
}

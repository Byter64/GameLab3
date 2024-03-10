#include "Systems/BulletSystem.h"
#include "Engine.h"
#include "ECSExtension.h"

void BulletSystem::EntityAdded(Engine::Entity entity)
{

}

void BulletSystem::EntityRemoved(Engine::Entity entity)
{

}

void BulletSystem::Update(float deltaTime)
{
    for (Engine::Entity entity : entities)
    {
        Bullet& bullet = ecsSystem->GetComponent<Bullet>(entity);
        Engine::Transform& transform = ecsSystem->GetComponent<Engine::Transform>(entity);
        Engine::BoxCollider& collider = ecsSystem->GetComponent<Engine::BoxCollider>(entity);

        for(auto pair : collider.collisions)
        {
            const Engine::Collision& collision = pair.first;
            if(collision.other != bullet.spawner)
            {
                ecsSystem->RemoveEntity(entity);
                break;
            }
        }

        transform.AddTranslation(glm::vec3(bullet.velocity, 0) * deltaTime);
    }
}
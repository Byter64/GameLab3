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

        for(auto& collision : collider.collisions)
        {
            if(collision.other != bullet.spawner)
            {
                Engine::RemoveEntityWithChildren(entity);

                if(ecsSystem->HasComponent<PlayerController>(bullet.spawner))
                    ecsSystem->GetComponent<PlayerController>(bullet.spawner).activeBullets--;
                break;
            }
        }

        transform.AddTranslation(glm::vec3(bullet.velocity, 0) * deltaTime);

        if(bullet.isSprite)
        {
            glm::quat rot = Engine::Systems::renderSystem->camera.GetRotation();
            rot = glm::inverse(rot);
            rot *= glm::quat(glm::vec3(glm::radians(90.0f), 0.0f, 0.0f));
            transform.SetRotation(rot);
        }
    }
}

void BulletSystem::RemoveAllBullets()
{
    for(Engine::Entity entity : entities)
        Engine::RemoveEntityWithChildren(entity);
}

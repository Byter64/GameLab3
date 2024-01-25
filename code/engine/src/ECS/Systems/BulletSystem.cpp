#include "ECS/Systems/BulletSystem.h"
#include "Engine.h"
namespace Engine
{
    void BulletSystem::EntityAdded(Entity entity)
    {

    }

    void BulletSystem::EntityRemoved(Entity entity)
    {

    }

    void BulletSystem::Update(float deltaTime)
    {
        for (Entity entity : entities)
        {
            Bullet& bullet = ecsSystem->GetComponent<Bullet>(entity);
            Transform& transform = ecsSystem->GetComponent<Transform>(entity);
            BoxCollider& collider = ecsSystem->GetComponent<BoxCollider>(entity);

            for(auto pair : collider.collisions)
            {
                const Collision& collision = pair.first;
                if(collision.other != bullet.spawner)
                {
                    ecsSystem->RemoveEntity(entity);
                    break;
                }
            }

            transform.AddTranslation(glm::vec3(bullet.velocity, 0) * deltaTime);
        }
    }
} // Engine
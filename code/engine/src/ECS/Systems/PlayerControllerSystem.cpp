#include "ECS/Systems/PlayerControllerSystem.h"
#include "ECSSystem.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/PlayerController.h"
#include "Engine.h"
#include <iostream>

extern Engine::ECSSystem* ecsSystem;
namespace Engine
{
    void PlayerControllerSystem::EntityAdded(Entity entity)
    {

    }

    void PlayerControllerSystem::EntityRemoved(Entity entity)
    {

    }

    void PlayerControllerSystem::Update(float deltaTime)
    {
        for (Entity entity: entities)
        {
            Transform& transform = ecsSystem->GetComponent<Transform>(entity);
            PlayerController& controller = ecsSystem->GetComponent<PlayerController>(entity);
            BoxCollider& boxCollider = ecsSystem->GetComponent<BoxCollider>(entity);

            for(auto pair : boxCollider.collisions)
            {
                Transform& otherTransform = ecsSystem->GetComponent<Transform>(pair.first.other);
                glm::vec3 distance = transform.GetGlobalTranslation() - otherTransform.GetGlobalTranslation();
                if(distance == glm::vec3(0))
                    distance.x += 1;
                transform.AddTranslation(glm::normalize(distance) * controller.speed * 2.0f * deltaTime);
            }

            if(controller.movementInput != glm::vec3(0))
            {
                transform.AddTranslation(glm::normalize(glm::vec3(controller.movementInput)) * deltaTime * controller.speed);
                controller.lookDirection = glm::normalize(glm::vec3(controller.movementInput));
            }
            if(controller.wasFirePushed)
            {
                controller.wasFirePushed = false;
                SpawnBullet(entity);
            }

            std::cout << transform.ToString() << std::endl;
        }
    }

    void PlayerControllerSystem::SpawnBullet(Entity player)
    {
        Engine::PlayerController& controller =ecsSystem->GetComponent<Engine::PlayerController>(player);
        Entity entity = ImportGLTF(Engine::Files::ASSETS / "Graphics\\Models\\Bullet\\Bullet.glb")[0];
        ecsSystem->GetComponent<Engine::Transform>(entity).SetScale(glm::vec3(0.2f));
        ecsSystem->GetComponent<Engine::Transform>(entity).SetTranslation(ecsSystem->GetComponent<Engine::Transform>(player).GetGlobalTranslation());

        Engine::Bullet& bullet = ecsSystem->AddComponent<Engine::Bullet>(entity);
        bullet.velocity = controller.lookDirection * controller.speed * 2.0f;
        bullet.spawner = player;

        BoxCollider& collider = ecsSystem->AddComponent<Engine::BoxCollider>(entity);
        collider.size = glm::vec3(0.4f, 0.6f, 0.4f);
        collider.isStatic = false;
        collider.collisions.clear();
    }
} // Engine
#include "ECS/Systems/PlayerControllerSystem.h"
#include "ECSSystem.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/PlayerController.h"
#include "Engine.h"
#include <iostream>

extern Engine::ECSSystem* ecsSystem;
extern GLFWwindow* window;
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
                //If wall
                Transform& otherTransform = ecsSystem->GetComponent<Transform>(pair.first.other);
                glm::vec3 distance = transform.GetGlobalTranslation() - otherTransform.GetGlobalTranslation();
                if(distance == glm::vec3(0))
                    distance.x += 1;
                transform.AddTranslation(glm::normalize(distance) * controller.speed * 2.0f * deltaTime);

                //If bullet
                Entity other = pair.first.other;
                if(ecsSystem->HasComponent<Bullet>(other) && ecsSystem->GetComponent<Bullet>(other).spawner != entity)
                {
                    //Bullet is already destroying itself, so no need to do it here
                    Health& health = ecsSystem->GetComponent<Health>(entity);
                    health.health--;
                    if(health.health <= 0)
                    {
                        std::cout << "Player dead" << std::endl;
                        //glfwSetWindowShouldClose(window, true);
                    }
                }
            }

            if(controller.movementInput != glm::vec3(0))
            {
                transform.AddTranslation(glm::normalize(glm::vec3(controller.movementInput)) * deltaTime * controller.speed);
                controller.lookDirection = glm::normalize(glm::vec3(controller.movementInput));
            }
            if(controller.wasFirePushed)
            {
                controller.wasFirePushed = false;
                SpawnBullet(entity, transform.GetGlobalTranslation(), controller.lookDirection);
            }
        }
    }
} // Engine
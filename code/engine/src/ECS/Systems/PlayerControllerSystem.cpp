#include "ECS/Systems/PlayerControllerSystem.h"
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
        for (Entity playerEntity: entities)
        {
            ResolveCollisions(playerEntity, deltaTime);
            HandleInput(playerEntity, deltaTime);
            UpdateUI(playerEntity);
        }
    }

    void PlayerControllerSystem::ResolveCollisions(Entity playerEntity, float deltaTime)
    {
        Transform& transform = ecsSystem->GetComponent<Transform>(playerEntity);
        PlayerController& controller = ecsSystem->GetComponent<PlayerController>(playerEntity);
        BoxCollider& boxCollider = ecsSystem->GetComponent<BoxCollider>(playerEntity);

        for(auto pair : boxCollider.collisions)
        {
            //If wall was hit
            Transform& otherTransform = ecsSystem->GetComponent<Transform>(pair.first.other);
            glm::vec3 distance = transform.GetGlobalTranslation() - otherTransform.GetGlobalTranslation();
            if(distance == glm::vec3(0))
                distance.x += 1;
            transform.AddTranslation(glm::normalize(distance) * controller.speed * 2.0f * deltaTime);

            //If bullet was hit
            Entity other = pair.first.other;
            if(ecsSystem->HasComponent<Bullet>(other) && ecsSystem->GetComponent<Bullet>(other).spawner != playerEntity)
            {
                //Bullet is already destroying itself, so no need to do it here
                Health& health = ecsSystem->GetComponent<Health>(playerEntity);
                health.health--;
                if(health.health <= 0)
                {
                    std::cout << "Player dead" << std::endl;
                    glfwSetWindowShouldClose(window, true);
                }
            }
        }
    }

    void PlayerControllerSystem::HandleInput(Entity playerEntity, float deltaTime)
    {
        Transform& transform = ecsSystem->GetComponent<Transform>(playerEntity);
        PlayerController& controller = ecsSystem->GetComponent<PlayerController>(playerEntity);

        if(controller.movementInput != glm::vec3(0))
        {
            transform.AddTranslation(glm::normalize(glm::vec3(controller.movementInput)) * deltaTime * controller.speed);
            controller.lookDirection = glm::normalize(glm::vec3(controller.movementInput));
        }
        if(controller.wasFirePushed)
        {
            controller.wasFirePushed = false;
            SpawnBullet(playerEntity, transform.GetGlobalTranslation(), controller.lookDirection);
        }
    }

    void PlayerControllerSystem::UpdateUI(Entity playerEntity)
    {
        PlayerController& controller = ecsSystem->GetComponent<PlayerController>(playerEntity);
        if(controller.hasScoreChanged && ecsSystem->HasComponent<Text>(controller.uiTextScore))
        {
            Text& uiText = ecsSystem->GetComponent<Text>(controller.uiTextScore);
            uiText.SetText(std::to_string(controller.score));
            controller.hasScoreChanged = false;
        }
    }
} // Engine
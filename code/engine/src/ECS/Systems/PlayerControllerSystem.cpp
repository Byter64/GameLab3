#include "ECS/Systems/PlayerControllerSystem.h"
#include "ECSSystem.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/PlayerController.h"

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
            Engine::Transform& transform = ecsSystem->GetComponent<Engine::Transform>(entity);
            Engine::PlayerController& controller = ecsSystem->GetComponent<Engine::PlayerController>(entity);

            if(controller.movementInput != glm::vec2(0,0))
                transform.AddTranslation(glm::normalize(glm::vec3(controller.movementInput, 0)) * deltaTime *
                controller.speed);
        }
    }
} // Engine
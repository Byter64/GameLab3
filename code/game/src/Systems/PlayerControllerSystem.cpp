#include "Engine.h"
#include <iostream>
#include "ECSExtension.h"
#include "Systems/PlayerControllerSystem.h"


static const float SQRT_THREE_FOURTH = 0.866025f;

void PlayerControllerSystem::EntityAdded(Engine::Entity entity)
{
    alivePlayers++;
}

void PlayerControllerSystem::EntityRemoved(Engine::Entity entity)
{
    alivePlayers--;
    if(alivePlayers == 0)
    {
        std::cout << "All players dead" << std::endl;
        Engine::EndGame();
    }
}

void PlayerControllerSystem::Update(float deltaTime)
{
    for (Engine::Entity playerEntity: entities)
    {
        HandleInput(playerEntity, deltaTime);
        ResolveCollisions(playerEntity, deltaTime);
        UpdateUI(playerEntity);
    }
}

void PlayerControllerSystem::ResolveCollisions(Engine::Entity playerEntity, float deltaTime)
{
    Engine::Transform& transform = ecsSystem->GetComponent<Engine::Transform>(playerEntity);
    Engine::BoxCollider& boxCollider = ecsSystem->GetComponent<Engine::BoxCollider>(playerEntity);
    PlayerController& controller = ecsSystem->GetComponent<PlayerController>(playerEntity);

    glm::vec3 averageDirection{0};
    int count = 0;
    for(auto pair : boxCollider.collisions)
    {
        Engine::Entity other = pair.first.other;
        if(ecsSystem->HasComponent<Loot>(other))
        {
            controller.AddScore(ecsSystem->GetComponent<Loot>(other).score);
            RemoveEntityWithChildren(other);
        }
        //If bullet was hit
        else if (ecsSystem->HasComponent<Bullet>(other) && ecsSystem->GetComponent<Bullet>(other).spawner != playerEntity)
        {
            //If bullet is shot from player
            if (ecsSystem->HasComponent<PlayerController>(ecsSystem->GetComponent<Bullet>(other).spawner))
            {
                controller.stunnedTimer = stunnedTime;
            }
            else
            {
                //Bullet is already destroying itself, so no need to do it here
                Health &health = ecsSystem->GetComponent<Health>(playerEntity);
                health.health--;
                if (health.health <= 0)
                {
                    Engine::RemoveEntityWithChildren(playerEntity);
                }
            }
        }
        else
        {
            Engine::Transform &otherTransform = ecsSystem->GetComponent<Engine::Transform>(other);
            glm::vec3 distance = transform.GetGlobalTranslation() - otherTransform.GetGlobalTranslation();
            averageDirection += distance;
            count++;
        }
    }

    if(count != 0)
    {
        averageDirection = RoundToAxis(averageDirection);

        if (glm::length(controller.movementInput) >= inputDeadzone)
            averageDirection *= glm::length(controller.movementInput) * controller.speed;
        transform.AddTranslation(averageDirection * deltaTime);
    }
}

void PlayerControllerSystem::HandleInput(Engine::Entity playerEntity, float deltaTime)
{
    Engine::Transform& transform = ecsSystem->GetComponent<Engine::Transform>(playerEntity);
    PlayerController& controller = ecsSystem->GetComponent<PlayerController>(playerEntity);

    if(controller.stunnedTimer > 0)
    {
        controller.stunnedTimer -= deltaTime;
        return;
    }

    if(glm::length(controller.movementInput) >= inputDeadzone)
    {
        transform.AddTranslation(glm::vec3(controller.movementInput) * deltaTime * controller.speed);
        controller.lookDirection = glm::normalize(glm::vec3(controller.movementInput));
    }
    if(controller.wasFirePushed)
    {
        controller.wasFirePushed = false;
        ECSHelper::SpawnBullet(playerEntity, transform.GetGlobalTranslation(), RoundToAxis(controller.lookDirection), 6);
    }
}

void PlayerControllerSystem::UpdateUI(Engine::Entity playerEntity)
{
    PlayerController& controller = ecsSystem->GetComponent<PlayerController>(playerEntity);
    if(controller.hasScoreChanged && ecsSystem->HasComponent<Engine::Text>(controller.uiTextScore))
    {
        Engine::Text& uiText = ecsSystem->GetComponent<Engine::Text>(controller.uiTextScore);
        uiText.SetText(std::to_string(controller.score));
        controller.hasScoreChanged = false;
    }
}

glm::vec3 PlayerControllerSystem::RoundToAxis(glm::vec3 vec)
{
    vec = glm::normalize(vec);
    vec *= SQRT_THREE_FOURTH; //Normalize
    vec.x = roundf(vec.x);
    vec.y = roundf(vec.y);
    vec.z = roundf(vec.z);

    vec = glm::normalize(vec);
    return vec;
}

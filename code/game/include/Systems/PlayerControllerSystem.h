#pragma once
#include "ECS/System.h"

class PlayerControllerSystem : public Engine::System
{
    const float inputDeadzone = 0.05f;

    unsigned int alivePlayers = 0;
    inline static const float respawnDistance = 1.5f;
    void ResolveCollisions(Engine::Entity playerEntity, float deltaTime);
    void HandleInput(Engine::Entity playerEntity, float deltaTime);
    void UpdateUI(Engine::Entity playerEntity);
    glm::vec3 RoundToAxis(glm::vec3 vec);
    void DeactivatePlayer(Engine::Entity entity);
    void ActivatePlayer(Engine::Entity entity);
public:
    void EntityAdded(Engine::Entity entity) override;
    void EntityRemoved(Engine::Entity entity) override;
    void Update(float deltaTime);
};

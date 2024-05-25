#pragma once
#include "ECS/System.h"

class PlayerControllerSystem : public Engine::System
{
    const float inputDeadzone = 0.05f;

    inline static const float respawnDistance = 1.5f;
    void ResolveCollisions(Engine::Entity playerEntity, float deltaTime);
    void HandleInput(Engine::Entity playerEntity, float deltaTime);
    void UpdateUI(Engine::Entity playerEntity);
    void DeactivatePlayer(Engine::Entity entity);
    void ActivatePlayer(Engine::Entity entity);
    int SumUpAllScores();
    void CheckIfAllPlayerDead();
public:
    Engine::Entity scoreUI;

    void EntityAdded(Engine::Entity entity) override;
    void EntityRemoved(Engine::Entity entity) override;
    void Update(float deltaTime);
};

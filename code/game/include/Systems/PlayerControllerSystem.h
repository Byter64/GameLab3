#pragma once
#include "ECS/System.h"

class PlayerControllerSystem : public Engine::System
{
    const float stunnedTime = 5.0f;

    void ResolveCollisions(Engine::Entity playerEntity, float deltaTime);
    void HandleInput(Engine::Entity playerEntity, float deltaTime);
    void UpdateUI(Engine::Entity playerEntity);
public:
    void EntityAdded(Engine::Entity entity) override;
    void EntityRemoved(Engine::Entity entity) override;
    void Update(float deltaTime);
};

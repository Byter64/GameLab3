#pragma once
#include "Engine.h"
#include "GameDefines.h"

class CuballSystem : public Engine::System
{
    void EntityAdded(Engine::Entity entity) override;
    void EntityRemoved(Engine::Entity entity) override;

    void Update(Engine::Entity entity, float deltaTime);
    void HandleDamage(Engine::Entity entity, Engine::Entity other);

    void MoveCuball(Engine::Entity entity, float deltaTime);

public:
    CuballSystem();

    void Update(float deltaTime);
};

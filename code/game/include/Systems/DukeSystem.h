#pragma once
#include "Engine.h"

class DukeSystem : public Engine::System
{
    void EntityAdded(Engine::Entity entity);
    void EntityRemoved(Engine::Entity entity);

    void Update(Engine::Entity entity, float deltaTime);
    void HandleDamage(Engine::Entity entity, Engine::Entity other);

public:
    DukeSystem();

    void Update(float deltaTime);
};

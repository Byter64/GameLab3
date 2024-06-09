#pragma once
#include "Engine.h"

class KindredSpiritSystem : public Engine::System
{
    static inline float HEIGHT = -0.5f;
    void EntityAdded(Engine::Entity entity);
    void EntityRemoved(Engine::Entity entity);

    void Update(Engine::Entity entity, float deltaTime);
    void HandleDamage(Engine::Entity entity, Engine::Entity other);

public:
    KindredSpiritSystem();
    void Update(float deltaTime);
};

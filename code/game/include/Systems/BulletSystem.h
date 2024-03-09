#pragma once
#include "ECS/System.h"
class BulletSystem : public Engine::System
{
    void EntityAdded(Engine::Entity entity) override;
    void EntityRemoved(Engine::Entity entity) override;

public:
    void Update(float deltaTime);
};

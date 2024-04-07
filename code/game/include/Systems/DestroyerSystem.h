#pragma once
#include "Engine.h"

class DestroyerSystem : public Engine::System
{
    void EntityAdded(Engine::Entity entity) override;
    void EntityRemoved(Engine::Entity entity) override;

public:
    void Update();
};

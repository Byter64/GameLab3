#pragma once
#include "Engine.h"

class HubertusSystem : public Engine::System
{
    inline static std::pair<float, float> idleDurationRange;
    inline static std::pair<float, float> walkDurationRange;
    inline static std::pair<float, float> shootIntervalRange;

    void EntityAdded(Engine::Entity entity) override;
    void EntityRemoved(Engine::Entity entity) override;

    void Update(Engine::Entity entity, float deltaTime);
    void HandleDamage(Engine::Entity entity, Engine::Entity other);

public:
    HubertusSystem();

    void Update(float deltaTime);
};

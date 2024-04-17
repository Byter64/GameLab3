#pragma once
#include "Engine.h"
#include "Helpers/Movement.h"
#include "Components/Enemies/Assi.h"

class AssiSystem : public Engine::System
{
    inline static std::pair<float, float> idleDurationRange;

    void EntityAdded(Engine::Entity entity) override;
    void EntityRemoved(Engine::Entity entity) override;

    void Update(Engine::Entity entity, float deltaTime);
    void HandleDamage(Engine::Entity entity, Engine::Entity other);
    void Move(Engine::Entity entity, float deltaTime);

public:
    AssiSystem();

    void Update(float deltaTime);
};

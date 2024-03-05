#pragma once
#include "ECS/System.h"

namespace Engine
{

    class PlayerControllerSystem : public System
    {
        void ResolveCollisions(Entity playerEntity, float deltaTime);
        void HandleInput(Entity playerEntity, float deltaTime);
        void UpdateUI(Entity playerEntity);
    public:
        void EntityAdded(Entity entity) override;
        void EntityRemoved(Entity entity) override;
        void Update(float deltaTime);
    };

} // Engine

#pragma once
#include "ECS/System.h"
namespace Engine
{

    class BulletSystem : public System
    {
        void EntityAdded(Entity entity) override;
        void EntityRemoved(Entity entity) override;

    public:
        void Update(float deltaTime);
    };

} // Engine

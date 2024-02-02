#pragma once
#include "ECS/System.h"

namespace Engine
{

    class PlayerControllerSystem : public System
    {
    public:
        void EntityAdded(Entity entity);
        void EntityRemoved(Entity entity);
        void Update(float deltaTime);
    };

} // Engine

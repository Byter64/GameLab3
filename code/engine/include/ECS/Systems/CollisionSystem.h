#pragma once
#include "Engine.h"

namespace Engine
{
    class CollisionSystem : public Engine::System
    {
        void CleanExitCollision(Entity entity);
        void CheckCollision(Entity entity1, Entity entity2);

    public:
        void CheckCollisions();
    };

} // Engine

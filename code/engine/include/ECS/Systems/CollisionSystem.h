#pragma once
#include "Engine.h"

namespace Engine
{
    class CollisionSystem : public Engine::System
    {
        void CleanExitCollision(Entity entity);
        void CleanEnterCollision(Entity entity);
        void CheckCollision(Entity entity1, Entity entity2);

        void EntityAdded(Entity entity) override;
        void EntityRemoved(Entity entity) override;
    public:
        void CheckCollisions();
    };

} // Engine

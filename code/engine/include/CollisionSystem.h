#pragma once
#include "Engine.h"
#include <set>

namespace Engine
{
    class CollisionSystem : public Engine::System
    {
        struct Collision
        {
            Entity entity1;
            Entity entity2;

            bool operator==(const Collision& other);
            bool operator!=(const Collision& other);
            bool operator <(const Collision& other) const;
        };

        std::set<Collision> activeCollisions;

        void CheckCollision(Entity entity1, Entity entity2);

    public:
        void CheckCollisions();
    };

} // Engine

#pragma once
#include "Engine.h"
#include <map>
#include <set>

namespace Engine
{
    class CollisionSystem : public System
    {
        //Contains all layers which should not collide with the given layer
        std::map<unsigned char, std::set<unsigned char>> ignoredLayers;
        void CleanExitCollision(Entity entity);
        void CleanEnterCollision(Entity entity);
        void CheckCollision(Entity entity1, Entity entity2);

        void EntityAdded(Entity entity) override;
        void EntityRemoved(Entity entity) override;
    public:
        void CheckCollisions();
        void SetCollisionBetweenLayers(unsigned char layer1, unsigned char layer2, bool canCollide);
    };

} // Engine

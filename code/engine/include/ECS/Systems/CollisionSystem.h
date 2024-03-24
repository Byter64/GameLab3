#pragma once
#include "ECS/System.h"
#include <map>
#include <set>
#include "ECS/Components/BoxCollider.h"

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
        bool CheckCollision(BoxCollider const &collider1, BoxCollider const &collider2);
        void SetCollisionBetweenLayers(unsigned char layer1, unsigned char layer2, bool canCollide);
    };

} // Engine

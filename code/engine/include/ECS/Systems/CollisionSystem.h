#pragma once
#include "ECS/System.h"
#include <map>
#include <set>
#include "ECS/Components/BoxCollider.h"
#include "ECS/Components/TilemapCollider.h"

namespace Engine
{
    class CollisionSystem : public System
    {
        std::set<Entity> entitiesWithBoxColliders;
        std::set<Entity> entitiesWithTilemapColliders;
        //Contains all layers which should not collide with the given layer
        std::map<unsigned char, std::set<unsigned char>> ignoredLayers;

        void EntityAdded(Entity entity) override;
        void EntityRemoved(Entity entity) override;

        void CleanExitCollision(BoxCollider& collider);
        void CleanExitCollision(TilemapCollider& collider);

        void CleanEnterCollision(BoxCollider& collider);
        void CleanEnterCollision(TilemapCollider& collider);

        void UpdateCollision(BoxCollider &collider1, BoxCollider &collider2, bool areColliding);
        void UpdateCollision(TilemapCollider &collider1, BoxCollider &collider2, bool areColliding);

    public:
        void CheckCollisions();
        bool CheckCollision(BoxCollider const &collider1, BoxCollider const &collider2);
        bool CheckCollision(TilemapCollider const &collider1, BoxCollider const &collider2);
        void SetCollisionBetweenLayers(unsigned char layer1, unsigned char layer2, bool canCollide);
    };

} // Engine

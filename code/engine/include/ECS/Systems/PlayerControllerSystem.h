#pragma once
#include "ECS/System.h"

namespace Engine
{

    class PlayerControllerSystem : public System
    {
        Entity bulletPrefab;

        void SpawnBullet(Entity player);

    public:
        PlayerControllerSystem();
        void EntityAdded(Entity entity);
        void EntityRemoved(Entity entity);
        void Update(float deltaTime);
    };

} // Engine

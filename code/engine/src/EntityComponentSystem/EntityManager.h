#pragma once
#include "Entity.h"
#include <queue>
#include <array>

namespace Engine
{

    class EntityManager
    {
        std::queue<Entity> pooledEntities;
        std::array<Signature, MAX_ENTITIES> signatures;
        Entity activeEntitiesCount; //Currently used entities (these are always inside the pool)
        Entity pooledEntitiesCount; //Currently used AND unused entities

    public:
        EntityManager();
        Entity CreateEntity();
        void DestroyEntity(Entity Entity);
        void SetSignature(Entity entity, Signature signature);
        Signature GetSignature(Entity entity);
        bool AddNewEntitiesToPool(unsigned int amount);
    };

} // Engine

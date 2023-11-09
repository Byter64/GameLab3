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
        void DestroyEntity(Entity entity);
        void SetSignature(Entity entity, Signature signature);
        Signature GetSignature(Entity entity);

        /**
         * Adds 0 to amount entities to the pool.
         * @param amount The amount of entities that are tried to be added to the pool
         * @return Returns True, if at least 1 entity could be added, else false
         */
        bool AddNewEntitiesToPool(unsigned int amount);
    };

} // Engine

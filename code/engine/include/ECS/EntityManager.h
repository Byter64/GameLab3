#pragma once
#include "Entity.h"
#include <queue>
#include <array>
#include <set>

namespace Engine
{

    class EntityManager
    {
        std::queue<Entity> pooledEntities; //Unused entities that are available for usage
        std::array<Signature, Entity::MAX_ENTITIES> signatures;
        std::uint32_t activeEntitiesCount; //Currently used entities (these are always inside the pool)
        std::uint32_t pooledEntitiesCount; //Currently used AND unused entities
        std::set<Entity> purgatory;
        /**
         * Adds 0 to amount entities to the pool.
         * @param amount The amount of entities that are tried to be added to the pool
         * @return Returns True, if at least 1 entity could be added, else false
         */
        bool AddNewEntitiesToPool(unsigned int amount);

    public:
        EntityManager();
        Entity CreateEntity();
        void SetSignature(Entity entity, Signature signature);
        Signature GetSignature(Entity entity);
        void RemoveEntity(Entity entity);
        void DestroyEntity(Entity entity);
        void DeletePurgatory();
    };

} // Engine

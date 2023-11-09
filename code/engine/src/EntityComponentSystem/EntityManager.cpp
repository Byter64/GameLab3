#include "EntityManager.h"

namespace Engine
{
    EntityManager::EntityManager()
    {
        activeEntitiesCount = 0;
        pooledEntitiesCount = 0;

        AddNewEntitiesToPool(100);
    }

    bool EntityManager::AddNewEntitiesToPool(unsigned int amount)
    {
        unsigned int unpooledEntities = MAX_ENTITIES - pooledEntitiesCount;

        amount = amount > unpooledEntities ? unpooledEntities : amount;

        if(amount == 0)
        {
            return false;
        }

        for(int i = pooledEntitiesCount; i < pooledEntitiesCount + amount; i++)
        {
            pooledEntities.push(pooledEntitiesCount + i);
        }
        pooledEntitiesCount += amount;

        return true;
    }
} // Engine
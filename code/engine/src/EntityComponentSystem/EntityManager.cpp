#include "../../include/EntityManager.h"

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

        if(amount == 0) return false;

        for(int i = pooledEntitiesCount; i < pooledEntitiesCount + amount; i++)
        {
            pooledEntities.push(pooledEntitiesCount + i);
        }
        pooledEntitiesCount += amount;

        return true;
    }

    Entity EntityManager::CreateEntity()
    {
        if(activeEntitiesCount == pooledEntitiesCount)
        {
            bool hasWorked = AddNewEntitiesToPool(100);
            if(!hasWorked) return INVALID_ENTITY_ID;
        }

        activeEntitiesCount++;

        Entity id = pooledEntities.front();
        pooledEntities.pop();
        return id;
    }

    void EntityManager::DestroyEntity(Entity entity)
    {
        if(entity == INVALID_ENTITY_ID) return;

        signatures[entity].reset();
        pooledEntities.push(entity);
        activeEntitiesCount--;
    }

    void EntityManager::SetSignature(Entity entity, Signature signature)
    {
        if(entity == INVALID_ENTITY_ID) return;

        signatures[entity] = signature;
    }

    Signature EntityManager::GetSignature(Entity entity)
    {
        if(entity == INVALID_ENTITY_ID) return 0;

        return signatures[entity];
    }
} // Engine
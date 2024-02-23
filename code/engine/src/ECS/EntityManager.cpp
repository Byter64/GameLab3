#include "ECS/EntityManager.h"
#include "ECSSystem.h"

extern Engine::ECSSystem* ecsSystem;
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
        unsigned int unpooledEntities = Entity::MAX_ENTITIES - pooledEntitiesCount;

        amount = amount > unpooledEntities ? unpooledEntities : amount;

        if(amount == 0) return false;

        for(int i = 0; i < amount; i++)
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
            if(!hasWorked) return Entity::INVALID_ENTITY_ID;
        }

        activeEntitiesCount++;

        Entity id = pooledEntities.front();
        pooledEntities.pop();
        
        return id;
    }

    void EntityManager::RemoveEntity(Entity entity)
    {
        purgatory.insert(entity);
    }

    void EntityManager::SetSignature(Entity entity, Signature signature)
    {
        if(entity == Entity::INVALID_ENTITY_ID) return;

        signatures[entity.id] = signature;
    }

    Signature EntityManager::GetSignature(Entity entity)
    {
        if(entity == Entity::INVALID_ENTITY_ID) return 0;

        return signatures[entity.id];
    }

    void EntityManager::DestroyEntity(Entity entity)
    {
        if(entity == Entity::INVALID_ENTITY_ID) return;

        signatures[entity.id].reset();
        pooledEntities.push(entity);
        activeEntitiesCount--;
    }

    void EntityManager::DeletePurgatory()
    {
        for(Entity entity : purgatory)
        {
            ecsSystem->DestroyEntity(entity);
        }
        purgatory.clear();
    }
} // Engine
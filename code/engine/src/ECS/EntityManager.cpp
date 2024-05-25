#include "ECS/EntityManager.h"
#include "ECSSystem.h"

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
            pooledEntities.push_back(pooledEntitiesCount + i);
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
        pooledEntities.pop_front();
        
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
        pooledEntities.push_back(entity);
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

    void EntityManager::RemoveAllEntities()
    {
        for(int i = 0; i < pooledEntitiesCount; i++)
        {
            auto iter = std::find(pooledEntities.begin(), pooledEntities.end(), i);
            if(iter == pooledEntities.end())
                RemoveEntity(i);
        }
    }

    bool EntityManager::IsEntityActive(Entity entity)
    {
        return std::find(pooledEntities.begin(), pooledEntities.end(),entity) == pooledEntities.end();
    }
} // Engine
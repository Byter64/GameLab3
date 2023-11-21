#include "ECSSystem.h"

namespace Engine
{
    void ECSSystem::Init()
    {
        entityManager = std::make_unique<EntityManager>();
        componentManager = std::make_unique<ComponentManager>();
        systemManager = std::make_unique<SystemManager>();
    }

    Entity ECSSystem::CreateEntity()
    {
        return entityManager->CreateEntity();
    }

    void ECSSystem::DestroyEntity(Entity entity)
    {
        entityPurgatory.insert(entity);
    }

    void ECSSystem::DestroyEntitiesForReal()
    {
        for(Entity entity : entityPurgatory)
        {
            entityManager->DestroyEntity(entity);
            componentManager->EntityDestroyed(entity);
            systemManager->EntityDestroyed(entity);
        }

        entityPurgatory.clear();
    }

    Signature ECSSystem::GetSignature(Entity entity)
    {
        return entityManager->GetSignature(entity);
    }
} // Engine
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

    void ECSSystem::DeleteEntity(Entity entity)
    {
        systemManager->EntityDestroyed(entity);
        entityManager->DeleteEntity(entity);
        componentManager->EntityDestroyed(entity);
    }

    Signature ECSSystem::GetSignature(Entity entity)
    {
        return entityManager->GetSignature(entity);
    }

    void ECSSystem::DeletePurgatory()
    {
        entityManager->DeletePurgatory();
    }

    /*
     * Returns the component type with the highest ID.
     */
    ComponentType ECSSystem::GetHighestComponentType() const
    {
        return componentManager->GetHighestComponentType();
    }
} // Engine
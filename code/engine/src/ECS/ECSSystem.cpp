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

    /*
     * You usually dont want to use this, as it immediately destroys the entity.
     * You should rather use RemoveEntity, which removes the entity at the end of the frame
     */
    void ECSSystem::DestroyEntity(Entity entity)
    {
        systemManager->EntityDestroyed(entity);
        entityManager->DestroyEntity(entity);
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
     * Removes the entity at the end of the frame
     */
    void ECSSystem::RemoveEntity(Entity entity)
    {
        entityManager->RemoveEntity(entity);
    }
} // Engine
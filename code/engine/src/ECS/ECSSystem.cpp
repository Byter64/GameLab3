#include "ECSSystem.h"
#include "Systems.h"

std::shared_ptr<Engine::ECSSystem>& ecsSystem = Engine::Systems::ecsSystem;

namespace Engine
{
    void ECSSystem::Init()
    {
        entityManager = std::make_unique<EntityManager>();
        componentManager = std::make_unique<ComponentManager>();
        systemManager = std::make_unique<SystemManager>();
    }

    void ECSSystem::Uninit()
    {
        systemManager.reset();
        componentManager.reset();
        entityManager.reset();
;    }

    Entity ECSSystem::CreateEntity()
    {
        return entityManager->CreateEntity();
    }

    /*
     * You usually don't want to use this, as it immediately destroys the entity.
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

    ComponentType ECSSystem::GetNumberOfRegisteredComponents()
    {
        return componentManager->GetNumberOfRegisteredComponents();
    }

    const char *ECSSystem::GetComponentTypeName(ComponentType componentType)
    {
        return componentManager->GetTypeName(componentType);
    }

    /// Runtime component check. ONLY USE IT IF YOU KNOW WHAT YOU ARE DOING
    /// \param entity
    /// \return
    bool ECSSystem::HasComponent(Entity entity, ComponentType componentType)
    {
        Signature signature = entityManager->GetSignature(entity);
        return signature[componentType];
    }

    void ECSSystem::RemoveAllEntities()
    {
        entityManager->RemoveAllEntities();
    }

    bool ECSSystem::IsEntityActive(Entity entity)
    {
        return entityManager->IsEntityActive(entity);
    }
} // Engine
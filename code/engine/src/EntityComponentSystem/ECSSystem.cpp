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
        entityManager->DestroyEntity(entity);
        componentManager->EntityDestroyed(entity);
        systemManager->EntityDestroyed(entity);
    }
} // Engine
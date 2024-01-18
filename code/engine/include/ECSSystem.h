#pragma once
#include <memory>
#include <set>
#include "ECS/EntityManager.h"
#include "ECS/ComponentManager.h"
#include "ECS/SystemManager.h"
namespace Engine
{

    class ECSSystem
    {
        std::unique_ptr<EntityManager> entityManager;
        std::unique_ptr<ComponentManager> componentManager;
        std::unique_ptr<SystemManager> systemManager;

        std::set<Entity> entityPurgatory;

    public:
        void Init();

        Entity CreateEntity();
        void DeleteEntity(Entity entity);
        void DeletePurgatory();

        ComponentType GetHighestComponentType() const;

        template<typename T>
        void RegisterComponent()
        {
            componentManager->RegisterComponent<T>();
        }

        //Component data is undefined!!!
        template<typename T>
        T& AddComponent(Entity entity)
        {
            T& component = componentManager->AddComponent<T>(entity);

            auto signature = entityManager->GetSignature(entity);
            signature.set(componentManager->GetComponentType<T>(), true);
            entityManager->SetSignature(entity, signature);

            systemManager->EntitySignatureChanged(entity, signature);

            return component;
        }

        template<typename T>
        void AddComponent(Entity entity, T component)
        {
            componentManager->AddComponent<T>(entity, component);

            auto signature = entityManager->GetSignature(entity);
            signature.set(componentManager->GetComponentType<T>(), true);
            entityManager->SetSignature(entity, signature);

            systemManager->EntitySignatureChanged(entity, signature);
        }

        template<typename T>
        void RemoveComponent(Entity entity)
        {
            componentManager->RemoveComponent<T>(entity);

            auto signature = entityManager->GetSignature(entity);
            signature.set(componentManager->GetComponentType<T>(), false);
            entityManager->SetSignature(entity, signature);

            systemManager->EntitySignatureChanged(entity, signature);
        }

        template<typename T>
        T& GetComponent(Entity entity)
        {
            return componentManager->GetComponent<T>(entity);
        }
        
        template<typename T>
        bool HasComponent(Entity entity)
        {
            return componentManager->HasComponent<T>(entity);
        }

        Signature GetSignature(Entity entity);

        template<typename T>
        Entity GetEntity(T& component)
        {
            return componentManager->GetEntity(component);
        }


        template<typename T>
        ComponentType GetComponentType()
        {
            return componentManager->GetComponentType<T>();
        }

        template<typename T>
        std::shared_ptr<T> RegisterSystem()
        {
            return systemManager->RegisterSystem<T>();
        }

        template<typename T>
        void SetSystemSignature(Signature signature)
        {
                systemManager->SetSignature<T>(signature);
        }
    };

} // Engine

extern Engine::ECSSystem* ecsSystem;

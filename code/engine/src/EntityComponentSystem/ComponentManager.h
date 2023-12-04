#pragma once
#include <unordered_map>
#include <memory>
#include "ComponentType.h"
#include "IComponentArray.h"
#include "ComponentArray.h"


namespace Engine
{

    class ComponentManager
    {
        std::unordered_map<const char*, ComponentType> componentTypes;
        std::unordered_map<const char*, std::shared_ptr<IComponentArray>> componentArrays;

        ComponentType nextFreeComponentType;

        template<typename T>
        std::shared_ptr<ComponentArray<T>> GetComponentArray()
        {
            const char* typeName = typeid(T).name();

            assert(componentTypes.find(typeName) != componentTypes.end() && "This component has not been registered yet");

            return std::static_pointer_cast<ComponentArray<T>>(componentArrays[typeName]);
        }

    public:
        template<typename T>
        void RegisterComponent()
        {
            const char* typeName = typeid(T).name();

            assert(componentTypes.find(typeName) == componentTypes.end() && "This component has already been registered");

            componentArrays.insert({typeName, std::make_shared<ComponentArray<T>>()});

            componentTypes.insert({typeName, nextFreeComponentType});
            nextFreeComponentType++;
        }

        template<typename T>
        ComponentType GetComponentType()
        {
            const char* typeName = typeid(T).name();

            assert(componentTypes.find(typeName) != componentTypes.end() && "This component has not been registered yet");

            return componentTypes[typeName];
        }

        //Component data is undefined!!!
        template<typename T>
        void AddComponent(Entity entity)
        {
            GetComponentArray<T>()->AddComponent(entity);
        }

        template<typename T>
        void AddComponent(Entity entity, T component)
        {
            GetComponentArray<T>()->AddComponent(entity, component);
        }

        template<typename T>
        void RemoveComponent(Entity entity)
        {
            GetComponentArray<T>()->RemoveComponent(entity);
        }

        template<typename T>
        T& GetComponent(Entity entity)
        {
            return GetComponentArray<T>()->GetComponent(entity);
        }

        template<typename T>
        bool HasComponent(Entity entity)
        {
            return GetComponentArray<T>()->HasComponent(entity);
        }

        template<typename T>
        Entity GetEntity(T& component)
        {
            return GetComponentArray<T>()->GetEntity(component);
        }

        void EntityDestroyed(Entity entity)
        {
            for(auto const& pair: componentArrays)
            {
                auto const& component = pair.second;
                component->EntityDestroyed(entity);
            }
        }
    };

} // Engine

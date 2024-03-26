#pragma once
#include <unordered_map>
#include <memory>
#include <stdexcept>
#include "ComponentType.h"
#include "IComponentArray.h"
#include "ComponentArray.h"


namespace Engine
{

    class ComponentManager
    {
        std::unordered_map<const char*, ComponentType> componentTypes;
        std::unordered_map<const char*, std::shared_ptr<IComponentArray>> typeToComponentArrays;
        std::unordered_map<ComponentType, std::shared_ptr<IComponentArray>> componentTypeToComponentArrays;

        ComponentType nextFreeComponentType;

        template<typename T>
        std::shared_ptr<ComponentArray<T>> GetComponentArray()
        {
            const char* typeName = typeid(T).name();

            if(componentTypes.find(typeName) == componentTypes.end())
            {
                std::string message{"Component \""};
                message += typeName;
                message += "\" has not been registered yet";
                throw std::runtime_error(message);
            }

            return std::static_pointer_cast<ComponentArray<T>>(typeToComponentArrays[typeName]);
        }

    public:
        template<typename T>
        void RegisterComponent()
        {
            const char* typeName = typeid(T).name();

            if(componentTypes.find(typeName) != componentTypes.end())
            {
                std::string message{"Component \""};
                message += typeName;
                message += "\" has already been registered";
                throw std::runtime_error(message);
            }

            std::shared_ptr<IComponentArray> array = std::make_shared<ComponentArray<T>>();
            typeToComponentArrays.insert({typeName, array});
            componentTypeToComponentArrays.insert({nextFreeComponentType, array});
            componentTypes.insert({typeName, nextFreeComponentType});
            nextFreeComponentType++;
        }

        template<typename T>
        ComponentType GetComponentType()
        {
            const char* typeName = typeid(T).name();

            if(componentTypes.find(typeName) == componentTypes.end())
            {
                std::string message{"Component \""};
                message += typeName;
                message += "\" has not been registered yet";
                throw std::runtime_error(message);
            }

            return componentTypes[typeName];
        }

        //Component data is undefined!!!
        template<typename T>
        T& AddComponent(Entity entity)
        {
            return GetComponentArray<T>()->AddComponent(entity);
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
        int GetNumberOfComponents()
        {
            return GetComponentArray<T>()->GetNumberOfComponents();
        }

        template<typename T>
        Entity GetEntity(T const& component)
        {
            return GetComponentArray<T>()->GetEntity(component);
        }

        void EntityDestroyed(Entity entity)
        {
            for(auto const& pair: typeToComponentArrays)
            {
                auto const& component = pair.second;
                component->EntityDestroyed(entity);
            }
        }
    };

} // Engine

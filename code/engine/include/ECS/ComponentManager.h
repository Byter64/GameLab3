#pragma once
#include <unordered_map>
#include <memory>
#include <stdexcept>
#include "ComponentType.h"
#include "IComponentArray.h"
#include "ComponentArray.h"
#include "EngineException.h"

namespace Engine
{

    class ComponentManager
    {
        std::unordered_map<const char*, ComponentType> nameToType;
        std::unordered_map<ComponentType, const char*> typeToName;
        std::unordered_map<const char*, std::shared_ptr<IComponentArray>> typeToComponentArrays;
        std::unordered_map<ComponentType, std::shared_ptr<IComponentArray>> componentTypeToComponentArrays;

        ComponentType nextFreeComponentType;

        template<typename T>
        std::shared_ptr<ComponentArray<T>> GetComponentArray()
        {
            const char* typeName = typeid(T).name();

            if(nameToType.find(typeName) == nameToType.end())
            {
                std::string message{"Component \""};
                message += typeName;
                message += "\" has not been registered yet";
                throw EngineException("COMPONENT NOT REGISTERED", message);
            }

            return std::static_pointer_cast<ComponentArray<T>>(typeToComponentArrays[typeName]);
        }

    public:
        template<typename T>
        void RegisterComponent()
        {
            const char* typeName = typeid(T).name();

            if(nameToType.find(typeName) != nameToType.end())
            {
                std::string message{"Component \""};
                message += typeName;
                message += "\" has already been registered";
                throw EngineException("COMPONENT ALREADY REGISTERED", message);
            }

            std::shared_ptr<IComponentArray> array = std::make_shared<ComponentArray<T>>();
            typeToComponentArrays.insert({typeName, array});
            componentTypeToComponentArrays.insert({nextFreeComponentType, array});
            nameToType.insert({typeName, nextFreeComponentType});
            typeToName.insert({nextFreeComponentType, typeName});
            nextFreeComponentType++;
        }

        template<typename T>
        ComponentType GetComponentType()
        {
            const char* typeName = typeid(T).name();

            if(nameToType.find(typeName) == nameToType.end())
            {
                std::string message{"Component \""};
                message += typeName;
                message += "\" has not been registered yet";
                throw EngineException("COMPONENT NOT REGISTERED", message);
            }

            return nameToType[typeName];
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

        const char *GetTypeName(ComponentType componentType);
        ComponentType GetNumberOfRegisteredComponents();
    };

} // Engine

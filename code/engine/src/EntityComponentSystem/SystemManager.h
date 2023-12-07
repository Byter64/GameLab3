#pragma once
#include <unordered_map>
#include "System.h"
#include "Entity.h"
#include <memory>
#include <stdexcept>

namespace Engine
{

    class SystemManager
    {
        std::unordered_map<const char*, Signature> signatures;
        std::unordered_map<const char*, std::shared_ptr<System>> systems;

    public:
        template<typename T>
        std::shared_ptr<T> RegisterSystem()
        {
            const char* typeName = typeid(T).name();

            if(systems.find(typeName) != systems.end())
            {
                std::string errorMessage{"System \""};
                errorMessage += typeName;
                errorMessage += "\" has already been registered";
                throw std::runtime_error(errorMessage);
            }

            auto system = std::make_shared<T>();
            systems.insert({typeName, system});
            return system;
        }

        template<typename T>
        void SetSignature(Signature signature)
        {
            const char* typeName = typeid(T).name();

            if(systems.find(typeName) == systems.end())
            {
                std::string message{"System \""};
                message += typeName;
                message += "\" has not been registered yet";
                throw std::runtime_error(message);
            }

            signatures.insert({typeName, signature});
        }

    void EntityDestroyed(Entity entity);
    void EntitySignatureChanged(Entity entity, Signature signature);
    };

} // Engine

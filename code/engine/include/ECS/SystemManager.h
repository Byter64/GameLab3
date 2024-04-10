#pragma once
#include <unordered_map>
#include "System.h"
#include "Entity.h"
#include <memory>
#include <stdexcept>
#include "EngineException.h"

namespace Engine
{

    class SystemManager
    {
        std::unordered_map<const char*, std::list<Signature>> signatures;
        std::unordered_map<const char*, std::shared_ptr<System>> systems;

    public:
        template<typename T>
        std::shared_ptr<T> RegisterSystem()
        {
            const char* typeName = typeid(T).name();

            if(systems.find(typeName) != systems.end())
            {
                std::string message{"System \""};
                message += typeName;
                message += "\" has already been registered";
                throw EngineException("SYSTEM ALREADY REGISTERED", message);
            }

            auto system = std::make_shared<T>();
            systems.insert({typeName, system});
            return system;
        }

        template<typename T>
        void AddSignature(Signature signature)
        {
            const char* typeName = typeid(T).name();

            if(systems.find(typeName) == systems.end())
            {
                std::string message{"System \""};
                message += typeName;
                message += "\" has not been registered yet";
                throw EngineException("SYSTEM NOT REGISTERED", message);
            }

            signatures[typeName].push_back(signature);
        }

    void EntityDestroyed(Entity entity);
    void EntitySignatureChanged(Entity entity, Signature signature);
    };

} // Engine

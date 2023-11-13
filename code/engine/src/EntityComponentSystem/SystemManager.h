#pragma once
#include <unordered_map>
#include "System.h"
#include "Entity.h"
#include <memory>
#include <assert.h>

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

            assert(systems.find(typeName) == systems.end() && "System has already been registered");

            auto system = std::make_shared<T>();
            systems.insert({typeName, system});
            return system;
        }

        template<typename T>
        void SetSignature(Signature signature)
        {
            const char* typeName = typeid(T).name();

            assert(systems.find(typeName) != systems.end() && "System has not been registered yet");

            signatures.insert({typeName, signature});
        }

    void EntityDestroyed(Entity entity);
    void EntitySignatureChanged(Entity entity, Signature signature);
    };

} // Engine

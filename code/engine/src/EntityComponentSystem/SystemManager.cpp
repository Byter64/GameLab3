#include "SystemManager.h"

namespace Engine
{
    void SystemManager::EntityDestroyed(Entity entity)
    {
        for(auto const& pair : systems)
        {
            auto const& system = pair.second;

            system->entities.erase(entity);
        }
    }

    void SystemManager::EntitySignatureChanged(Entity entity, Signature signature)
    {
        for(auto const& pair: systems)
        {
            auto const& type = pair.first;
            auto const& system = pair.second;
            auto const& systemSignature = signatures[type];

            //original comparison: (signature & systemSignature) == systemSignature
            if((systemSignature &signature) == systemSignature)
            {
                system->entities.insert(entity);
            }
            else
            {
                system->entities.erase(entity);
            }
        }
    }
} // Engine
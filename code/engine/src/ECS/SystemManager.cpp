#include "ECS/SystemManager.h"
#include <algorithm>

namespace Engine
{
    void SystemManager::EntityDestroyed(Entity entity)
    {
        for(auto const& pair : systems)
        {
            auto const& system = pair.second;
            bool wasInSystem = system->entities.find(entity) != system->entities.end();
            system->entities.erase(entity);
            if(wasInSystem)
                system->EntityRemoved(entity);
        }
    }

    void SystemManager::EntitySignatureChanged(Entity entity, Signature signature)
    {
        for(auto const& pair: systems)
        {
            auto const& type = pair.first;
            auto const& system = pair.second;
            auto const& systemSignature = signatures[type];

            bool doesSignatureFit = std::any_of(systemSignature.cbegin(), systemSignature.cend(), [&](Signature const& systemSignature){return (systemSignature & signature) == systemSignature; });
            if(doesSignatureFit)
            {
                if(system->entities.find(entity) == system->entities.end())
                    system->EntityAdded(entity);
                system->entities.insert(entity);
            }
            else
            {
                bool wasInSystem = system->entities.find(entity) != system->entities.end();
                system->entities.erase(entity);
                if(wasInSystem)
                    system->EntityRemoved(entity);
            }
        }
    }
} // Engine
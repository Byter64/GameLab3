#include "GameEventSystem.h"
#include "ECSSystem.h"
#include "GameEvents.h"

extern Engine::ECSSystem ecsSystem;
namespace Engine
{
    void GameEventSystem::InvokeUpdate(float time)
    {
        for(Entity entity : entities)
        {
            ecsSystem.GetComponent<GameEvents>(entity).update(time);
        }
    }
} // Engine
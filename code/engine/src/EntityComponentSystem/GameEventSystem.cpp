#include "../../include/GameEventSystem.h"
#include "../../include/ECSSystem.h"
#include "../../include/GameEventHolder.h"

extern Engine::ECSSystem ecsSystem;
namespace Engine
{
    void GameEventSystem::InvokeUpdate(float time)
    {
        for(Entity entity : entities)
        {
            ecsSystem.GetComponent<GameEventHolder>(entity).update(time);
        }
    }
} // Engine
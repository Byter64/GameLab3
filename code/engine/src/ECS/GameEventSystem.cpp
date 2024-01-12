#include "ECS/Systems/GameEventSystem.h"
#include "../../include/ECSSystem.h"
#include "ECS/Components/GameEvents.h"

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
#pragma once
#include <memory>
#include "ECS/Systems/RenderSystem.h"
#include "ECS/Systems/CollisionSystem.h"
#include "ECS/Systems/TextRenderSystem.h"
#include "InputSystem/InputSystem.h"
#include "ECSSystem.h"
#include "TimeManager.h"

namespace Engine::Systems
{
    extern std::shared_ptr<ECSSystem> ecsSystem;
    extern std::shared_ptr<InputSystem> inputSystem;
    extern std::shared_ptr<RenderSystem> renderSystem;
    extern std::shared_ptr<CollisionSystem> collisionSystem;
    extern std::shared_ptr<TextRenderSystem> textRenderSystem;
    extern std::shared_ptr<TimeManager> timeManager;
} // Engine::Systems

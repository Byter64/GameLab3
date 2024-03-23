#pragma once
#include "Systems.h"

namespace Engine::Systems
{
    std::shared_ptr<ECSSystem> ecsSystem;
    std::shared_ptr<InputSystem> inputSystem;
    std::shared_ptr<RenderSystem> renderSystem;
    std::shared_ptr<CollisionSystem> collisionSystem;
    std::shared_ptr<TextRenderSystem> textRenderSystem;
    std::shared_ptr<AnimationSystem> animationSystem;
    std::shared_ptr<TimeManager> timeManager;
} // Engine::Systems

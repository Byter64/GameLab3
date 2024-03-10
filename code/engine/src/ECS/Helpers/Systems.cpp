#pragma once
#include "ECS/Helpers/Systems.h"

namespace Engine::Systems
{
    std::shared_ptr<RenderSystem> renderSystem;
    std::shared_ptr<CollisionSystem> collisionSystem;
    std::shared_ptr<TextRenderSystem> textRenderSystem;
} // Engine::Systems

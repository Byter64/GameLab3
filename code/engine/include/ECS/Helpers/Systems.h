#pragma once
#include <memory>
#include "ECS/Systems/RenderSystem.h"
#include "ECS/Systems/CollisionSystem.h"
#include "ECS/Systems/TextRenderSystem.h"

namespace Engine::Systems
{
    extern std::shared_ptr<RenderSystem> renderSystem;
    extern std::shared_ptr<CollisionSystem> collisionSystem;
    extern std::shared_ptr<TextRenderSystem> textRenderSystem;
} // Engine::Systems

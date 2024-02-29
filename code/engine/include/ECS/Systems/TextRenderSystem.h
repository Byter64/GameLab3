#pragma once
#include "ECS/System.h"

namespace Engine
{

    class TextRenderSystem : public System
    {
        void EntityAdded(Engine::Entity entity) override;
        void EntityRemoved(Engine::Entity entity) override;

    public:
        void Initialize(int screenWidth, int screenHeight);
        void SetViewport(int screenWidth, int screenHeight);
        void Render();
    };

} // Engine

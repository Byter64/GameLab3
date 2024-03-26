#pragma once
#include "ECS/System.h"

namespace Engine
{

    class TextRenderSystem : public System
    {
        void EntityAdded(Engine::Entity entity) override;
        void EntityRemoved(Engine::Entity entity) override;
        std::pair<float, float> resolutionScale;
        std::pair<int, int> referenceResolution = {1920, 1080};
        std::pair<int, int> targetResolution;

    public:
        void Initialize(int screenWidth, int screenHeight);
        void SetViewport(int screenWidth, int screenHeight);
        void Render();
        void SetReferenceResolution(int width, int height);
    };

} // Engine

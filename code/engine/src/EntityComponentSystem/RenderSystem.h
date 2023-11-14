#pragma once
#include <memory>
#include <stack>
#include "glm/glm.hpp"
#include "System.h"
#include "ECSSystem.h"
#include "Transform.h"

namespace Engine
{

    class RenderSystem : System
    {
        void Render(Transform& transform, std::stack<glm::mat4x4>& matrixStack);
    public:
        void Render();
    };

} // Engine

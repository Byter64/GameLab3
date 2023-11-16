#pragma once
#include <memory>
#include <stack>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "System.h"
#include "ECSSystem.h"
#include "Transform.h"
#include "GLFW/glfw3.h"

namespace Engine
{

    class RenderSystem : System
    {
        GLuint defaultShader;

        void Render(Entity entity, std::stack<glm::mat4x4>& matrixStack);
    public:
        RenderSystem();

        glm::mat4x4 projectionMatrix = glm::perspective(glm::radians(90.0f), 1.0f, 1.0f, 1000.0f);
        Transform camera;

        void Render();
    };

} // Engine

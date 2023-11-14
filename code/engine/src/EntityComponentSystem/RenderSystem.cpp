#include "RenderSystem.h"
#include "Entity.h"
#include "ECSSystem.h"
#include "MeshRenderer.h"
#include "Transform.h"

extern Engine::ECSSystem ecsSystem;
namespace Engine
{

    void RenderSystem::Render()
    {
        for(Entity entity : entities)
        {
            Transform& transform = ecsSystem.GetComponent<Transform>(entity);
            if(transform.GetParent() != nullptr) continue;

            std::stack<glm::mat4x4> matrixStack;
            matrixStack.push(glm::mat4x4(1));
            Render(transform, matrixStack);
        }
    }

    void RenderSystem::Render(Transform& transform, std::stack<glm::mat4x4>& matrixStack)
    {
        matrixStack.push(matrixStack.top() * transform.GetMatrix());

        

        auto children = transform.GetChildren();
        for(Transform* child : children)
        {
            Render(*child, matrixStack);
        }

        matrixStack.pop();
    }
} // Engine
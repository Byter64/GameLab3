#include "Engine.h"
#include <list>
extern Engine::ECSSystem* ecsSystem;
namespace Engine
{
    void TransformParentSystem::EntityAdded(Entity entity)
    {

    }

    void TransformParentSystem::EntityRemoved(Entity entity)
     {
        Transform& transform = ecsSystem->GetComponent<Transform>(entity);
        
        if(transform.GetParent() != nullptr)
        {
            std::list<Transform *> &transformList = transform.GetParent()->GetChildren();
            transformList.remove(&transform);
        }

        std::list<Transform*>& children = transform.GetChildren();
        while(!children.empty())
        {
            children.front()->SetParent(nullptr);
        }
    }
} // Engine
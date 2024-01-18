#include "ECS/Systems/TransformParentSystem.h"
#include "ECSSystem.h"
#include <list>
extern Engine::ECSSystem ecsSystem;
namespace Engine
{
    void TransformParentSystem::EntityAdded(Entity entity)
    {

    }

    void TransformParentSystem::EntityRemoved(Entity entity)
    {
        //Does not provide a call operator?!?!?!?!
        Transform& transform = ecsSystem.GetComponent<Transform>(entity);

        if(transform.GetParent() != nullptr)
        {
            std::list<Transform *> &transformList = transform.GetParent()->GetChildren();
            transformList.remove(&transform);
        }

        for(Transform* child : transform.GetChildren())
        {
            child->SetParent(nullptr);
        }
    }
} // Engine
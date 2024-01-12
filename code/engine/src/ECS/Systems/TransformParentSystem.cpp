#include "ECS/Systems/TransformParentSystem.h"
#include "ECSSystem.h"
#include <list>
namespace Engine
{
    extern ECSSystem ecsSystem;
    void TransformParentSystem::EntityAdded(Entity entity)
    {

    }

    void TransformParentSystem::EntityRemoved(Entity entity)
    {
        Transform& transform = ecsSystem.GetComponent<Transform>(entity);
        std::list<Transform*>& transformList = transform.GetParent()->GetChildren();
        transformList.remove(&transform);

        for(Transform* child : transform.GetChildren())
        {
            child->SetParent(nullptr);
        }
    }
} // Engine
#include "ECS/Systems/AnimationSystem.h"

namespace Engine
{
    void AnimationSystem::EntityAdded(Entity entity)
    {

    }

    void AnimationSystem::EntityRemoved(Entity entity)
    {

    }

    void AnimationSystem::AddAnimation(Animation &animation)
    {
        animations[animation.name] = animation;
    }

    Animation &AnimationSystem::GetAnimation(std::string const& name)
    {
        return animations.at(name);
    }
} // Engine
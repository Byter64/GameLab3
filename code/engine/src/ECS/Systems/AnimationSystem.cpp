#include "ECS/Systems/AnimationSystem.h"
#include "Engine.h"

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

    void AnimationSystem::Update(float deltaTime)
    {
        for(Entity entity : entities)
        {
            Animator &animator = ecsSystem->GetComponent<Animator>(entity);
            Animation &animation = animations[animator.animationName];

            for (Animation::Action const &action: animation.actions)
            {
                for (Animation::Channel const &channel: action.channels)
                {
                    Entity effectedEntity = FindEntityInHierarchy(entity, channel.hierarchy);
                    if (effectedEntity == Entity::INVALID_ENTITY_ID) continue;
                    Transform &transform = ecsSystem->GetComponent<Transform>(effectedEntity);

                    if (channel.target == Animation::Channel::Target::Rotation)
                    {
                        auto iterator = channel.functionTo4D.upper_bound(animator.currentTime);
                        //if animator.currentTime is beyond the channel's animation
                        if (iterator == channel.functionTo4D.end())
                        {
                            std::advance(iterator, -1);
                            SetValue(transform, (*iterator).second, channel.target);
                            continue;
                        }

                        //if animator.currentTime is before the channel's animation
                        if (iterator == channel.functionTo4D.begin())
                        {
                            SetValue(transform, (*iterator).second, channel.target);
                            continue;
                        }

                        std::pair<float, glm::quat> const &upperBound = *iterator; //always greater than animator.currentTime
                        std::advance(iterator, -1);
                        std::pair<float, glm::quat> const &lowerBound = *iterator; // always smaller equal animator.currentTime
                        float interpolationValue =
                                (animator.currentTime - lowerBound.first) / (upperBound.first - lowerBound.first);
                        glm::quat value = Interpolate(lowerBound.second, upperBound.second, interpolationValue,
                                                      channel.interpolation);

                        SetValue(transform, value, channel.target);
                    }
                    else
                    {
                        auto iterator = channel.functionTo3D.upper_bound(animator.currentTime);
                        //if animator.currentTime is beyond the channel's animation
                        if (iterator == channel.functionTo3D.end())
                        {
                            std::advance(iterator, -1);
                            SetValue(transform, (*iterator).second, channel.target);
                            continue;
                        }

                        //if animator.currentTime is before the channel's animation
                        if (iterator == channel.functionTo3D.begin())
                        {
                            SetValue(transform, (*iterator).second, channel.target);
                            continue;
                        }

                        std::pair<float, glm::vec3> const &upperBound = *iterator; //always greater than animator.currentTime
                        std::advance(iterator, -1);
                        std::pair<float, glm::vec3> const &lowerBound = *iterator; // always smaller equal animator.currentTime
                        float interpolationValue =
                                (animator.currentTime - lowerBound.first) / (upperBound.first - lowerBound.first);

                        SetValue(transform, Interpolate(lowerBound.second, upperBound.second, interpolationValue,channel.interpolation), channel.target);
                    }
                }
            }
            animator.currentTime += deltaTime * animator.speed;
            if ((animator.speed >= 0 && animator.currentTime >= animation.endTime) || (animator.speed < 0 && animator.currentTime <= animation.startTime))
            {
                if (animator.isLooping)
                    animator.currentTime -= animation.duration;
                else
                    finishedAnimations.push_back(entity);
            }
        }

        for(Engine::Entity entity : finishedAnimations)
        {
            ecsSystem->RemoveComponent<Animator>(entity);
        }
        finishedAnimations.clear();
    }

    void AnimationSystem::PlayAnimation(Entity entity, const std::string &name, bool isLooping, float startTime, float speed)
    {
        if(animations.count(name) == 0)
        {
            std::string message = "The animation \"" + name + "\" does not exist";
            throw EngineException("Unknown Animation", message);
        }

        Animator& animator = ecsSystem->AddComponent<Animator>(entity);
        animator.isLooping = isLooping;
        animator.animationName = name;
        animator.speed = speed;
        animator.currentTime = startTime;
    }

    Entity AnimationSystem::FindEntityInHierarchy(Entity root, std::vector<unsigned int> hierarchy)
    {
        Transform *transform = &ecsSystem->GetComponent<Transform>(root);

        for(int i = 0; i < hierarchy.size(); i++)
        {
            if(transform->GetChildren().size() < hierarchy[i])
                return Entity::INVALID_ENTITY_ID;

            transform = transform->GetChild(hierarchy[i]);
        }

        return ecsSystem->GetEntity(*transform);
    }

    void AnimationSystem::SetValue(Transform &transform, glm::vec3 const& values, Animation::Channel::Target target)
    {
        switch (target)
        {
            case Animation::Channel::Target::Translation:
                transform.SetTranslation(values);
                break;
            case Animation::Channel::Target::Scale:
                transform.SetScale(values);
                break;
        }
    }

    void AnimationSystem::SetValue(Transform &transform, const glm::quat &values, Animation::Channel::Target target)
    {
        transform.SetRotation(values);
    }

    glm::vec3 AnimationSystem::Interpolate(glm::vec3 const &x, glm::vec3 const &y, float a, Animation::Channel::Interpolation interpolation)
    {
        switch (interpolation)
        {
            case Animation::Channel::Interpolation::Step:
                return x;
            case Animation::Channel::Interpolation::Linear:
                return glm::mix(x, y, a);
            case Animation::Channel::Interpolation::CubicSpline:
                return x;
            default:  return x;
        }
    }

    glm::quat AnimationSystem::Interpolate(const glm::quat &x, const glm::quat &y, float a, Animation::Channel::Interpolation interpolation)
    {
        switch (interpolation)
        {
            case Animation::Channel::Interpolation::Step:
                return x;
            case Animation::Channel::Interpolation::Linear:
                if(x.w < 0.1f)
                    return x;
                return glm::mix(x, y, a);
            case Animation::Channel::Interpolation::CubicSpline:
                return x;
            default: return x;
        }
    }
} // Engine
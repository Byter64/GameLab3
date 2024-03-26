#pragma once
#include "ECS/System.h"
#include <map>
#include "ECS/Helpers/Animation.h"
#include "ECS/Components/Transform.h"

namespace Engine
{

    class AnimationSystem : public System
    {
        std::map<std::string, Animation> animations;
        std::vector<Engine::Entity> finishedAnimations;
        void EntityAdded(Entity entity) override;
        void EntityRemoved(Entity entity) override;

        Entity FindEntityInHierarchy(Entity root, std::vector<unsigned int> hierarchy);
        glm::vec3 Interpolate(glm::vec3 const& x, glm::vec3 const& y, float a, Animation::Channel::Interpolation interpolation);
        glm::quat Interpolate(glm::quat const& x, glm::quat const& y, float a, Animation::Channel::Interpolation interpolation);
        void SetValue(Transform& transform, glm::vec3 const& values, Animation::Channel::Target target);
        void SetValue(Transform& transform, glm::quat const& values, Animation::Channel::Target target);
    public:
        void AddAnimation(Animation& animation);
        Animation& GetAnimation(std::string const& name);

        void Update(float deltaTime);

        void PlayAnimation(Entity entity, std::string const &name,  bool isLooping = false, float startTime = 0, float speed = 1);
    };

} // Engine

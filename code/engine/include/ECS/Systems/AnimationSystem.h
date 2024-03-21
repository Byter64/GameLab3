#pragma once
#include "ECS/System.h"
#include <map>
#include "ECS/Helpers/Animation.h"

namespace Engine
{

    class AnimationSystem : public System
    {
        void EntityAdded(Entity entity) override;
        void EntityRemoved(Entity entity) override;

        std::map<std::string, Animation> animations;

    public:
        void AddAnimation(Animation& animation);
        Animation& GetAnimation(std::string const& name);
    };

} // Engine

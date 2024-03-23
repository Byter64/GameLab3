#pragma once
#include "ECS/Helpers/Animation.h"

namespace Engine
{

    struct Animator
    {
        bool isLooping = false;
        float speed = 1.0f;
        float currentTime = 0.0f;
        std::string animationName;

    };

} // Engine

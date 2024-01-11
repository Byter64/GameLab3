#pragma once
#include "GameEvents.h"
namespace Engine
{

    struct GameEventHolder
    {
        GameEvents* gameEvents;
        //void (*update)(float time);
    };

} // Engine

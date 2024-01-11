#pragma once

namespace Engine
{

    struct GameEvents
    {
        void (*update)(float time);
    };

} // Engine

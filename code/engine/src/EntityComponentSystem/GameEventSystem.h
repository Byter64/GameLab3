#pragma once
#include "System.h"
namespace Engine
{

    class GameEventSystem : public System
    {
    public:
        void InvokeUpdate(float time);
    };

} // Engine

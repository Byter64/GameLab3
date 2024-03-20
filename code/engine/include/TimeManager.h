#pragma once
#include <chrono>

namespace Engine
{

    class TimeManager
    {
        std::chrono::system_clock::time_point startTime;

    public:
        TimeManager();
        float GetTimeSinceStartup();
    };

} // Engine

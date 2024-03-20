#include "TimeManager.h"

namespace Engine
{
    TimeManager::TimeManager()
    {
        startTime = std::chrono::system_clock::now();
    }

    //Returns the time since startup in seconds
    float TimeManager::GetTimeSinceStartup()
    {
        std::chrono::microseconds duration = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - startTime);
        return duration.count() / 1000000.0f;
    }
} // Engine
#pragma once
#include <chrono>

namespace Engine
{

    class TimeManager
    {
        std::chrono::system_clock::time_point startTime;
        std::chrono::system_clock::time_point startTimePause;

        float timePaused = 0.0f;
    public:
        TimeManager();
        float GetTimeSinceStartup();
        float GetTimeSinceStartupWithoutPauses();
        void OnPause();
        void OnContinue();
    };

} // Engine

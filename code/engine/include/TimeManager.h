#pragma once
#include <chrono>
#define FRAMETIME60FPS 16667 //In microseconds, this is around 60 fps
#define FRAMETIME144FPS 6944 //In microseconds, this is around 144 fps
#define FRAMETIME FRAMETIME144FPS

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

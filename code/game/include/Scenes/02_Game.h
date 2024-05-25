#pragma once
#include "ECSHelper.h"

class Game
{
    static inline Engine::Entity pauseText;
    static inline std::shared_ptr<Engine::InputActionButton> pause;
    static inline float pauseStartTime;
    static inline float pauseTextScale = 8;
    static inline std::list<Engine::Entity> entities{};
    static void PauseGame(void*);
public:
    static inline bool isRunning = false;
    static void Start();
    static void Update(float deltaTime);
    static void UpdateWithoutPause();
    static void End();
};

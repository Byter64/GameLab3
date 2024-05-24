#pragma once
#include "ECSHelper.h"

class Game {
    static inline Engine::Entity pauseText;
    static inline std::shared_ptr<Engine::InputActionButton> pause;
    static inline float pauseStartTime;
    static inline float pauseTextScale = 8;

    static void PauseGame(void*);
public:
    static void StartMainPartOfGame(int screenWidth, int screenHeight);
    static void Update(float deltaTime);
    static void UpdateWithoutPause();
};

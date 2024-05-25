#pragma once
#include "ECSHelper.h"

class Game : public Engine::Scene
{
    Engine::Entity pauseText;
    std::shared_ptr<Engine::InputActionButton> pause;
    float pauseStartTime;
    float pauseTextScale = 8;
    std::list<Engine::Entity> entities{};
    static void PauseGame(void*);
public:
    bool isRunning = false;
    void OnStart() override;
    void OnEnd() override;
    void Update(float deltaTime);
    void UpdateWithoutPause();
};

#pragma once
#include "ECSHelper.h"

class Game : public Engine::Scene
{
    Engine::Entity pauseText;
    std::shared_ptr<Engine::InputActionButton> pause;
    float pauseStartTime;
    float pauseTextScale = 8;
    static void PauseGame(void*);
public:
    static inline int scoreP1 = -1;
    static inline int scoreP2 = -1;

    void OnStart() override;
    void OnEnd() override;
    void OnUpdate(float deltaTime) override;
    void OnUpdateWithoutPause() override;
};

#pragma once
#include "Engine.h"
#include "Keyboard.h"

class GameLost : public Engine::Scene
{
    std::shared_ptr<Engine::InputActionButton> button1;
    std::shared_ptr<Engine::InputActionButton> button2;
    static void OnButton1Press(void * doesntmatter);
    static void OnButton2Press(void * doesntmatter);
    void CreateText(std::string text, int x, int y, int scale);
    static Keyboard* CreateKeyBoard(int joystickID, int x, int y);
public:
    void OnStart() override;
    void OnEnd() override;
    void OnUpdate(float deltaTme) override;

};

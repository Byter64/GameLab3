#pragma once
#include "ECSExtension.h"

class GameWin : public Engine::Scene
{
    std::shared_ptr<Engine::InputActionButton> button1;
    std::shared_ptr<Engine::InputActionButton> button2;
    static void OnButtonPress(void * doesntmatter);
public:
    void CreateText(std::string text, int x, int y, int scale);
    Engine::Entity CreateInputText(std::string startText, int x, int y, int scale);
    void OnStart() override;
    void OnEnd() override;
};

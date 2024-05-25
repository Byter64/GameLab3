#pragma once
#include "ECSExtension.h"

class GameWin
{
    static inline std::shared_ptr<Engine::InputActionButton> button1;
    static inline std::shared_ptr<Engine::InputActionButton> button2;
    static inline std::list<Engine::Entity> entities{};
    static void OnButtonPress(void * doesntmatter);
public:
    static void CreateText(std::string text, int x, int y, int scale);
    static void Start(int score);
    static void End();
};

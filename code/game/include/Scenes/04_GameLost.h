#pragma once
#include "ECSExtension.h"

class GameLost
{
    static inline std::shared_ptr<Engine::InputActionButton> button1;
    static inline std::shared_ptr<Engine::InputActionButton> button2;
    static inline std::list<Engine::Entity> entities{};
    static void OnButtonPress(void * doesntmatter);
public:
    static void CreateText(std::string text, int x, int y, int scale);
    static void Start(int player1Score, int player2Score = -1);
    static void End();

};

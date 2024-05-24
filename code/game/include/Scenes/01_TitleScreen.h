#pragma once
#include "ECSExtension.h"

class TitleScreen {
    static inline std::shared_ptr<Engine::InputActionButton> button1;
    static inline std::shared_ptr<Engine::InputActionButton> button2;
    static void StartGameOnButtonPress(void * doesntmatter);
public:
    static void CreateTitleScreenText(std::string text, int x, int y, int scale);
    static void StartTitlePartOfGame(int screenWidth, int screenHeight);

};

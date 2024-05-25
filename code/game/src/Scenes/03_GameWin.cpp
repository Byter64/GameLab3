#include "Scenes/03_GameWin.h"
#include "Scenes/01_TitleScreen.h"

extern int windowWidth, windowHeight;

void GameWin::CreateText(std::string text, int x, int y, int scale)
{
    Engine::Entity infoText = ecsSystem->CreateEntity();
    entities.push_back(infoText);
    Engine::Text& infoTextText = ecsSystem->AddComponent<Engine::Text>(infoText);
    infoTextText.scale = scale;
    infoTextText.position = {x, y};
    infoTextText.horizontalAlignment = Engine::Text::Center;
    infoTextText.verticalAlignment = Engine::Text::Center;
    infoTextText.SetText(text);
}

void GameWin::Start(int score)
{
    CreateText("Congratulation you won the game", windowWidth / 2, windowHeight / 2, 5);
    CreateText("You reached " + std::to_string(score) + " points", windowWidth / 2, 300, 9);
    CreateText("Try to be faster for a higher score!", windowWidth / 2, windowHeight / 2 + 200, 3);

    button1 = std::make_shared<Engine::InputActionButton>("Any button");
    button1->AddGamepadBinding(Engine::GamepadInputID(GLFW_JOYSTICK_1, GLFW_GAMEPAD_BUTTON_START, Engine::GamepadInputID::InputType::Button));
    button1->AddGamepadBinding(Engine::GamepadInputID(GLFW_JOYSTICK_1, GLFW_GAMEPAD_BUTTON_BACK, Engine::GamepadInputID::InputType::Button));
    button1->AddOnEnd((void *) 0, OnButtonPress);
    Engine::Systems::inputSystem->Add(button1);


    if(glfwJoystickPresent(GLFW_JOYSTICK_2))
    {
        button2 = std::make_shared<Engine::InputActionButton>("Any button");
        button2->AddGamepadBinding(Engine::GamepadInputID(GLFW_JOYSTICK_2, GLFW_GAMEPAD_BUTTON_START, Engine::GamepadInputID::InputType::Button));
        button2->AddGamepadBinding(Engine::GamepadInputID(GLFW_JOYSTICK_2, GLFW_GAMEPAD_BUTTON_BACK, Engine::GamepadInputID::InputType::Button));
        button2->AddOnEnd((void *) 0, OnButtonPress);
        Engine::Systems::inputSystem->Add(button2);
    }
}

void GameWin::OnButtonPress(void * doesntmatter)
{
    End();
    TitleScreen::Start();
}

void GameWin::End()
{
    while(!entities.empty())
    {
        ecsSystem->RemoveEntity(entities.back());
        entities.pop_back();
    }
    Engine::Systems::inputSystem->Remove(button1);
    Engine::Systems::inputSystem->Remove(button2);
}

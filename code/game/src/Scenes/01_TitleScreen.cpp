#include "Scenes/01_TitleScreen.h"
#include "Scenes/02_Game.h"

extern int windowWidth, windowHeight;

void TitleScreen::CreateText(std::string text, int x, int y, int scale)
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

void TitleScreen::Start()
{
    CreateText("Press any button to start", windowWidth / 2, windowHeight / 2, 5);
    CreateText("My Cool Game Lab III Game", windowWidth / 2, 300, 9);
    CreateText("Here would be the highscores if there were any", windowWidth / 2, windowHeight / 2 + 200, 3);

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

void TitleScreen::OnButtonPress(void * doesntmatter)
{
    End();
    Game::Start();
}

void TitleScreen::End()
{
    while(!entities.empty())
    {
        ecsSystem->RemoveEntity(entities.back());
        entities.pop_back();
    }
    Engine::Systems::inputSystem->Remove(button1);
    Engine::Systems::inputSystem->Remove(button2);
}
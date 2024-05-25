#include "Scenes/04_GameLost.h"
#include "Scenes/01_TitleScreen.h"

extern int windowWidth, windowHeight;

void GameLost::CreateText(std::string text, int x, int y, int scale)
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

void GameLost::Start(int player1Score, int player2Score)
{
    const char *winner = player1Score > player2Score ? "player 1" : "player 2";
    CreateText((std::string)"Congratulation " + winner + " won the game", windowWidth / 2, windowHeight / 2, 5);
    CreateText("Player 1 reached " + std::to_string(player1Score) + " points", windowWidth / 2, 300, 9);
    if(player2Score != -1)
        CreateText("Player 2 reached " + std::to_string(player2Score) + "points", windowWidth / 2, 400, 9);

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

void GameLost::OnButtonPress(void * doesntmatter)
{
    End();
    TitleScreen::Start();
}

void GameLost::End()
{
    while(!entities.empty())
    {
        ecsSystem->RemoveEntity(entities.back());
        entities.pop_back();
    }
    Engine::Systems::inputSystem->Remove(button1);
    Engine::Systems::inputSystem->Remove(button2);
}

#include "Scenes/01_TitleScreen.h"
#include "Scenes/02_Game.h"
#include "HighScoreManager.h"

extern int windowWidth, windowHeight;

void TitleScreen::CreateText(std::string text, int x, int y, int scale)
{
    Engine::Entity infoText = CreateEntity();
    Engine::Text& infoTextText = ecsSystem->AddComponent<Engine::Text>(infoText);
    infoTextText.scale = scale;
    infoTextText.position = {x, y};
    infoTextText.horizontalAlignment = Engine::Text::Center;
    infoTextText.verticalAlignment = Engine::Text::Center;
    infoTextText.SetText(text);
}

void TitleScreen::PrintScores()
{
    std::vector<HighScore> scores = HighScoreManager::ReadHighScores();

    CreateText(std::to_string(1) + ".", windowWidth * 2 / 5 - 250, windowHeight / 2 + 190 , 6);
    CreateText(std::to_string(scores[0].score), windowWidth * 2 / 5, windowHeight / 2 + 190, 6);
    CreateText(scores[0].teamName, windowWidth * 3 / 5, windowHeight / 2 + 190, 6);

    for(int i = 1; i < scores.size(); i++)
    {
        CreateText(std::to_string(i + 1) + ".", windowWidth * 2 / 5 - 250, windowHeight / 2 + 200 + i * 50, 3);
        CreateText(std::to_string(scores[i].score), windowWidth * 2 / 5, windowHeight / 2 + 200 + i * 50, 3);
        CreateText(scores[i].teamName, windowWidth * 3 / 5, windowHeight / 2 + 200+ i * 50, 3);
    }
}

void TitleScreen::OnStart()
{
    CreateText("My Cool Game Lab III Game", windowWidth / 2, 300, 9);
    CreateText("Press any button to start", windowWidth / 2, windowHeight / 2, 5);

    PrintScores();

    button1 = std::make_shared<Engine::InputActionButton>("Any button");
    button1->AddGamepadBinding(Engine::GamepadInputID(GLFW_JOYSTICK_1, GLFW_GAMEPAD_BUTTON_START, Engine::GamepadInputID::InputType::Button));
    button1->AddGamepadBinding(Engine::GamepadInputID(GLFW_JOYSTICK_1, GLFW_GAMEPAD_BUTTON_BACK, Engine::GamepadInputID::InputType::Button));
    button1->AddKeyboardBinding(GLFW_KEY_ENTER);
    button1->AddOnEnd((void *) 0, OnButtonPress);
    Engine::Systems::inputSystem->Add(button1);


    if(glfwJoystickPresent(GLFW_JOYSTICK_2))
    {
        button2 = std::make_shared<Engine::InputActionButton>("Any button");
        button2->AddGamepadBinding(Engine::GamepadInputID(GLFW_JOYSTICK_2, GLFW_GAMEPAD_BUTTON_START, Engine::GamepadInputID::InputType::Button));
        button2->AddGamepadBinding(Engine::GamepadInputID(GLFW_JOYSTICK_2, GLFW_GAMEPAD_BUTTON_BACK, Engine::GamepadInputID::InputType::Button));
        button2->AddKeyboardBinding(GLFW_KEY_ENTER);
        button2->AddOnEnd((void *) 0, OnButtonPress);
        Engine::Systems::inputSystem->Add(button2);
    }
}

void TitleScreen::OnButtonPress(void * doesntmatter)
{
    sceneManager->LoadScene<Game>();
}

void TitleScreen::OnEnd()
{
    Engine::Systems::inputSystem->Remove(button1);
    Engine::Systems::inputSystem->Remove(button2);
}

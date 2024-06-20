#include "Scenes/03_GameWin.h"
#include "Scenes/01_TitleScreen.h"
#include "Scenes/02_Game.h"
#include "Keyboard.h"
#include "HighScoreManager.h"

extern int windowWidth, windowHeight;
static Keyboard* keyboard;
static Engine::Entity teamName;
static int score;
void GameWin::CreateText(std::string text, int x, int y, int scale)
{
    Engine::Entity infoText = CreateEntity();
    Engine::Text& infoTextText = ecsSystem->AddComponent<Engine::Text>(infoText);
    infoTextText.scale = scale;
    infoTextText.position = {x, y};
    infoTextText.horizontalAlignment = Engine::Text::Center;
    infoTextText.verticalAlignment = Engine::Text::Center;
    infoTextText.SetText(text);
}

void GameWin::OnStart()
{
    score = Game::scoreP1;
    if(Game::scoreP2 != -1)
        score += Game::scoreP2;

    CreateText("Congratulations, You won!", windowWidth / 2, 300, 8);
    CreateText("You reached " + std::to_string(score) + " points", windowWidth / 2, 420, 5);
    CreateText("Try to be faster for a higher score!", windowWidth / 2, windowHeight - 50, 3);

    button1 = std::make_shared<Engine::InputActionButton>("Any button");
    button1->AddGamepadBinding(Engine::GamepadInputID(GLFW_JOYSTICK_1, GLFW_GAMEPAD_BUTTON_START, Engine::GamepadInputID::InputType::Button));
    button1->AddGamepadBinding(Engine::GamepadInputID(GLFW_JOYSTICK_1, GLFW_GAMEPAD_BUTTON_BACK, Engine::GamepadInputID::InputType::Button));
    button1->AddKeyboardBinding(GLFW_KEY_ENTER);
    button1->AddOnEnd((void *) 0, OnButtonPress);
    Engine::Systems::inputSystem->Add(button1);

    teamName = ecsSystem->CreateEntity();
    AddEntity(teamName);
    Engine::Text& text = ecsSystem->AddComponent<Engine::Text>(teamName);
    text.scale = 3;
    text.position = glm::vec2(800, 650);
    text.horizontalAlignment = Engine::Text::Left;

    keyboard = new Keyboard(glm::vec2(800, 750));
    auto temp = std::make_shared<Engine::InputActionButton>("");
    temp->AddGamepadBinding(Engine::GamepadInputID(GLFW_JOYSTICK_1, GLFW_GAMEPAD_BUTTON_DPAD_UP, Engine::GamepadInputID::InputType::Button));
    keyboard->SetUpAction(temp);
    temp = std::make_shared<Engine::InputActionButton>("");
    temp->AddGamepadBinding(Engine::GamepadInputID(GLFW_JOYSTICK_1, GLFW_GAMEPAD_BUTTON_DPAD_DOWN, Engine::GamepadInputID::InputType::Button));
    keyboard->SetDownAction(temp);
    temp = std::make_shared<Engine::InputActionButton>("");
    temp->AddGamepadBinding(Engine::GamepadInputID(GLFW_JOYSTICK_1, GLFW_GAMEPAD_BUTTON_DPAD_LEFT, Engine::GamepadInputID::InputType::Button));
    keyboard->SetLeftAction(temp);
    temp = std::make_shared<Engine::InputActionButton>("");
    temp->AddGamepadBinding(Engine::GamepadInputID(GLFW_JOYSTICK_1, GLFW_GAMEPAD_BUTTON_DPAD_RIGHT, Engine::GamepadInputID::InputType::Button));
    keyboard->SetRightAction(temp);
    temp = std::make_shared<Engine::InputActionButton>("");
    temp->AddGamepadBinding(Engine::GamepadInputID(GLFW_JOYSTICK_1, GLFW_GAMEPAD_BUTTON_A, Engine::GamepadInputID::InputType::Button));
    temp->AddGamepadBinding(Engine::GamepadInputID(GLFW_JOYSTICK_1, GLFW_GAMEPAD_BUTTON_B, Engine::GamepadInputID::InputType::Button));
    keyboard->SetEnterAction(temp);
    temp = std::make_shared<Engine::InputActionButton>("");
    temp->AddGamepadBinding(Engine::GamepadInputID(GLFW_JOYSTICK_1, GLFW_GAMEPAD_BUTTON_X, Engine::GamepadInputID::InputType::Button));
    temp->AddGamepadBinding(Engine::GamepadInputID(GLFW_JOYSTICK_1, GLFW_GAMEPAD_BUTTON_Y, Engine::GamepadInputID::InputType::Button));
    keyboard->SetDeleteAction(temp);

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

void GameWin::OnButtonPress(void * doesntmatter)
{
    sceneManager->LoadScene<TitleScreen>();
}

void GameWin::OnEnd()
{
    Engine::Systems::inputSystem->Remove(button1);
    Engine::Systems::inputSystem->Remove(button2);
    std::string name = keyboard->GetText();
    if(name.empty())
        name = "Anonymous";
    HighScoreManager::AddHighScore(score, name);
    delete keyboard;
}

void GameWin::OnUpdate(float deltaTime)
{
    ecsSystem->GetComponent<Engine::Text>(teamName).SetText(keyboard->GetText());
}

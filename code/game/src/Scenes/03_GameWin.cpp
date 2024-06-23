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
    button1->AddGamepadBinding(Engine::GamepadButton(0, GLFW_GAMEPAD_BUTTON_START));
    button1->AddGamepadBinding(Engine::GamepadButton(0, GLFW_GAMEPAD_BUTTON_BACK));
    button1->AddKeyboardBinding(GLFW_KEY_ENTER);
    button1->AddOnEnd((void *) 0, OnButtonPress);
    Engine::Systems::inputSystem->Add(button1);


    CreateText("Enter team name: ", 580, 675,3);
    teamName = ecsSystem->CreateEntity();
    AddEntity(teamName);
    Engine::Text& text = ecsSystem->AddComponent<Engine::Text>(teamName);
    text.scale = 3;
    text.position = glm::vec2(785, 650);
    text.horizontalAlignment = Engine::Text::Left;

    keyboard = new Keyboard(glm::vec2(800, 750));
    auto tempVec2 = std::make_shared<Engine::InputActionVec2>("");
    tempVec2->AddGamepadButtonBinding(Engine::GamepadButton(0, GLFW_GAMEPAD_BUTTON_DPAD_LEFT),
                                      Engine::GamepadButton(0, GLFW_GAMEPAD_BUTTON_DPAD_RIGHT),
                                      Engine::GamepadButton(0, GLFW_GAMEPAD_BUTTON_DPAD_UP),
                                      Engine::GamepadButton(0, GLFW_GAMEPAD_BUTTON_DPAD_DOWN));
    tempVec2->AddGamepadAxesBinding(Engine::GamepadAxis(0, GLFW_GAMEPAD_AXIS_LEFT_X),
                                    Engine::GamepadAxis(0, GLFW_GAMEPAD_AXIS_LEFT_Y));
    keyboard->SetMovementAction(tempVec2);
    auto tempButton = std::make_shared<Engine::InputActionButton>("");
    tempButton->AddGamepadBinding(Engine::GamepadButton(0, GLFW_GAMEPAD_BUTTON_A));
    tempButton->AddGamepadBinding(Engine::GamepadButton(0, GLFW_GAMEPAD_BUTTON_B));
    keyboard->SetEnterAction(tempButton);
    tempButton = std::make_shared<Engine::InputActionButton>("");
    tempButton->AddGamepadBinding(Engine::GamepadButton(0, GLFW_GAMEPAD_BUTTON_X));
    tempButton->AddGamepadBinding(Engine::GamepadButton(0, GLFW_GAMEPAD_BUTTON_Y));
    keyboard->SetDeleteAction(tempButton);

    if(Engine::Systems::inputSystem->IsGamepadPresent(1))
    {
        button2 = std::make_shared<Engine::InputActionButton>("Any button");
        button2->AddGamepadBinding(Engine::GamepadButton(1, GLFW_GAMEPAD_BUTTON_START));
        button2->AddGamepadBinding(Engine::GamepadButton(1, GLFW_GAMEPAD_BUTTON_BACK));
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

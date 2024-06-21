#include "Scenes/04_GameLost.h"
#include "Scenes/01_TitleScreen.h"
#include "Scenes/02_Game.h"
#include "Keyboard.h"
#include "HighScoreManager.h"

extern int windowWidth, windowHeight;
extern std::pair<Engine::Entity, Engine::Entity> players;
static Keyboard* keyboard1;
static Engine::Entity name1;
static Keyboard* keyboard2;
static Engine::Entity name2;
static bool isP1Ready = false;
static bool isP2Ready = false;
void GameLost::CreateText(std::string text, int x, int y, int scale)
{
    Engine::Entity infoText = CreateEntity();
    Engine::Text& infoTextText = ecsSystem->AddComponent<Engine::Text>(infoText);
    infoTextText.scale = scale;
    infoTextText.position = {x, y};
    infoTextText.horizontalAlignment = Engine::Text::Center;
    infoTextText.verticalAlignment = Engine::Text::Center;
    infoTextText.SetText(text);
}


Keyboard *GameLost::CreateKeyBoard(int gamepadId, int x, int y)
{
    Keyboard* keyboard = new Keyboard(glm::vec2(x, y));
    auto temp = std::make_shared<Engine::InputActionButton>("");
    temp->AddGamepadBinding(Engine::GamepadInputID(gamepadId, GLFW_GAMEPAD_BUTTON_DPAD_UP, Engine::GamepadInputID::InputType::Button));
    keyboard->SetUpAction(temp);
    temp = std::make_shared<Engine::InputActionButton>("");
    temp->AddGamepadBinding(Engine::GamepadInputID(gamepadId, GLFW_GAMEPAD_BUTTON_DPAD_DOWN, Engine::GamepadInputID::InputType::Button));
    keyboard->SetDownAction(temp);
    temp = std::make_shared<Engine::InputActionButton>("");
    temp->AddGamepadBinding(Engine::GamepadInputID(gamepadId, GLFW_GAMEPAD_BUTTON_DPAD_LEFT, Engine::GamepadInputID::InputType::Button));
    keyboard->SetLeftAction(temp);
    temp = std::make_shared<Engine::InputActionButton>("");
    temp->AddGamepadBinding(Engine::GamepadInputID(gamepadId, GLFW_GAMEPAD_BUTTON_DPAD_RIGHT, Engine::GamepadInputID::InputType::Button));
    keyboard->SetRightAction(temp);
    temp = std::make_shared<Engine::InputActionButton>("");
    temp->AddGamepadBinding(Engine::GamepadInputID(gamepadId, GLFW_GAMEPAD_BUTTON_A, Engine::GamepadInputID::InputType::Button));
    temp->AddGamepadBinding(Engine::GamepadInputID(gamepadId, GLFW_GAMEPAD_BUTTON_B, Engine::GamepadInputID::InputType::Button));
    keyboard->SetEnterAction(temp);
    temp = std::make_shared<Engine::InputActionButton>("");
    temp->AddGamepadBinding(Engine::GamepadInputID(gamepadId, GLFW_GAMEPAD_BUTTON_X, Engine::GamepadInputID::InputType::Button));
    temp->AddGamepadBinding(Engine::GamepadInputID(gamepadId, GLFW_GAMEPAD_BUTTON_Y, Engine::GamepadInputID::InputType::Button));
    keyboard->SetDeleteAction(temp);

    return keyboard;
}


void GameLost::OnStart()
{
    if(Engine::Systems::inputSystem->IsGamepadPresent(1))
    {
        const char *winner = Game::scoreP1 > Game::scoreP2 ? "player 1" : "player 2";
        CreateText((std::string) "Congratulation " + winner + " won the game", windowWidth / 2, 300, 6);
        CreateText("Player 2 reached " + std::to_string(Game::scoreP2) + "points", windowWidth / 2, 480, 4);
    }

    CreateText("Player 1 reached " + std::to_string(Game::scoreP1) + " points", windowWidth / 2, 400, 4);

    button1 = std::make_shared<Engine::InputActionButton>("Any button");
    button1->AddGamepadBinding(Engine::GamepadInputID(0, GLFW_GAMEPAD_BUTTON_START, Engine::GamepadInputID::InputType::Button));
    button1->AddGamepadBinding(Engine::GamepadInputID(0, GLFW_GAMEPAD_BUTTON_BACK, Engine::GamepadInputID::InputType::Button));
    button1->AddKeyboardBinding(GLFW_KEY_ENTER);
    button1->AddOnEnd((void *) 0, OnButton1Press);
    Engine::Systems::inputSystem->Add(button1);


    if(!Engine::Systems::inputSystem->IsGamepadPresent(1))
    {
        CreateText("Enter name: ", 640, 675,3);
        name1 = ecsSystem->CreateEntity();
        AddEntity(name1);
        Engine::Text& text = ecsSystem->AddComponent<Engine::Text>(name1);
        text.scale = 3;
        text.position = glm::vec2(785, 650);
        text.horizontalAlignment = Engine::Text::Left;

        keyboard1 = CreateKeyBoard(0, 800, 750);
    }

    if(Engine::Systems::inputSystem->IsGamepadPresent(1))
    {
        button2 = std::make_shared<Engine::InputActionButton>("Any button");
        button2->AddGamepadBinding(Engine::GamepadInputID(1, GLFW_GAMEPAD_BUTTON_START, Engine::GamepadInputID::InputType::Button));
        button2->AddGamepadBinding(Engine::GamepadInputID(1, GLFW_GAMEPAD_BUTTON_BACK, Engine::GamepadInputID::InputType::Button));
        button2->AddKeyboardBinding(GLFW_KEY_ENTER);
        button2->AddOnEnd((void *) 0, OnButton2Press);
        Engine::Systems::inputSystem->Add(button2);

        CreateText("P1, Enter name: ", 600, 700,3);
        name1 = ecsSystem->CreateEntity();
        AddEntity(name1);
        Engine::Text& text1 = ecsSystem->AddComponent<Engine::Text>(name1);
        text1.scale = 3;
        text1.position = glm::vec2(435, 750);
        text1.horizontalAlignment = Engine::Text::Left;
        keyboard1 = CreateKeyBoard(0, 450, 850);

        CreateText("P2, Enter name: ", 1250, 700,3);
        name2 = ecsSystem->CreateEntity();
        AddEntity(name2);
        Engine::Text& text2 = ecsSystem->AddComponent<Engine::Text>(name2);
        text2.scale = 3;
        text2.position = glm::vec2(1085, 750);
        text2.horizontalAlignment = Engine::Text::Left;
        keyboard2 = CreateKeyBoard(1, 1100, 850);
    }
}

void GameLost::OnButton1Press(void * doesntmatter)
{
    isP1Ready = !isP1Ready;
    if(isP1Ready)
    {
        delete keyboard1;
        keyboard1 = nullptr;
    }
    else
    {
        if(!Engine::Systems::inputSystem->IsGamepadPresent(1))
            keyboard1 = CreateKeyBoard(0, 800, 850);
        else
            keyboard1 = CreateKeyBoard(0, 450, 850);

        keyboard1->SetText(ecsSystem->GetComponent<Engine::Text>(name1).GetText());
    }
}

void GameLost::OnButton2Press(void * doesntmatter)
{
    isP2Ready = !isP2Ready;
    if(isP2Ready)
    {
        delete keyboard2;
        keyboard2 = nullptr;
    }
    else
    {
        keyboard2 = CreateKeyBoard(1, 1100, 850);

        keyboard2->SetText(ecsSystem->GetComponent<Engine::Text>(name2).GetText());
    }
}

void GameLost::OnEnd()
{
    Engine::Systems::inputSystem->Remove(button1);
    Engine::Systems::inputSystem->Remove(button2);

    HighScoreManager::AddHighScore(Game::scoreP1, ecsSystem->GetComponent<Engine::Text>(name1).GetText());
    if(Engine::Systems::inputSystem->IsGamepadPresent(1))
        HighScoreManager::AddHighScore(Game::scoreP2, ecsSystem->GetComponent<Engine::Text>(name2).GetText());

    delete keyboard1;
    delete keyboard2;
}

void GameLost::OnUpdate(float deltaTme)
{
    if(keyboard1 != nullptr)
        ecsSystem->GetComponent<Engine::Text>(name1).SetText(keyboard1->GetText());
    if(keyboard2 != nullptr)
        ecsSystem->GetComponent<Engine::Text>(name2).SetText(keyboard2->GetText());

    if(isP1Ready && isP2Ready)
        sceneManager->LoadScene<TitleScreen>();
}

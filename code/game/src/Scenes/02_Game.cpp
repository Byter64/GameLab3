#include "Scenes/02_Game.h"
#include "GameDefines.h"
#include "ECSExtension.h"

extern std::pair<Engine::Entity, Engine::Entity> players;

void Game::Start(int screenWidth, int screenHeight)
{
    pauseText = ecsSystem->CreateEntity();
    entities.push_back(pauseText);
    Engine::Text& pauseTextText = ecsSystem->AddComponent<Engine::Text>(pauseText);
    pauseTextText.scale = 0;
    pauseTextText.position = {screenWidth / 2, screenHeight / 2};
    pauseTextText.horizontalAlignment = Engine::Text::Center;
    pauseTextText.verticalAlignment = Engine::Text::Center;
    pauseTextText.SetText("Game Paused");

    Engine::Entity playersText = ecsSystem->CreateEntity();
    entities.push_back(playersText);
    auto& playersUI = ecsSystem->AddComponent<Engine::Text>(playersText);
    playersUI.scale = 4;
    playersUI.position = {890, 0};
    playersUI.SetText("Both");

    Engine::Entity playersUIActualScore = ecsSystem->CreateEntity();
    entities.push_back(playersUIActualScore);
    auto& playersTextUI = ecsSystem->AddComponent<Engine::Text>(playersUIActualScore);
    playersTextUI.scale = 4;
    playersTextUI.position = {960, 80};
    playersTextUI.horizontalAlignment = Engine::Text::Center;
    Systems::playerControllerSystem->scoreUI = playersUIActualScore;


    Engine::Entity player1Text = ecsSystem->CreateEntity();
    entities.push_back(player1Text);
    auto& player1UI = ecsSystem->AddComponent<Engine::Text>(player1Text);
    player1UI.scale = 4;
    player1UI.position = {200, 0};
    player1UI.SetText("Player 1");

    Engine::Entity playerUI = ecsSystem->CreateEntity();
    entities.push_back(playerUI);
    auto& textUI = ecsSystem->AddComponent<Engine::Text>(playerUI);
    textUI.scale = 4;
    textUI.position = {330, 80};
    textUI.horizontalAlignment = Engine::Text::Center;


    Engine::Entity player = Engine::ImportGLTF(Engine::Files::ASSETS / "Graphics\\Models\\Player.glb")[0];
    entities.push_back(player);
    ecsSystem->GetComponent<Engine::Name>(player) = "Player 1";
    PlayerController& controller = ecsSystem->AddComponent<PlayerController>(player);
    //Controller
    controller.uiTextScore = playerUI;
    controller.AddMovementInput(GLFW_JOYSTICK_1, GLFW_GAMEPAD_AXIS_LEFT_X, GLFW_GAMEPAD_AXIS_LEFT_Y);
    controller.AddMovementInput(GLFW_JOYSTICK_1, GLFW_GAMEPAD_BUTTON_DPAD_LEFT, GLFW_GAMEPAD_BUTTON_DPAD_RIGHT, GLFW_GAMEPAD_BUTTON_DPAD_UP, GLFW_GAMEPAD_BUTTON_DPAD_DOWN);
    controller.AddFireInput({GLFW_JOYSTICK_1, GLFW_GAMEPAD_BUTTON_A, Engine::GamepadInputID::Button});
    controller.AddFireInput({GLFW_JOYSTICK_1, GLFW_GAMEPAD_BUTTON_B, Engine::GamepadInputID::Button});
    controller.AddReviveInput({GLFW_JOYSTICK_1, GLFW_GAMEPAD_BUTTON_X, Engine::GamepadInputID::Button});
    controller.AddReviveInput({GLFW_JOYSTICK_1, GLFW_GAMEPAD_BUTTON_Y, Engine::GamepadInputID::Button});
    //Keyboard
    controller.AddMovementInput(GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_W, GLFW_KEY_S);
    controller.AddFireInput(GLFW_KEY_SPACE);
    controller.AddReviveInput(GLFW_KEY_SPACE);

    controller.speed = Defines::Float("Player1_Speed");
    controller.stunnedTime = Defines::Float("Player1_StunnedTime");
    controller.bulletSpeed = Defines::Float("Player1_BulletSpeed");
    controller.maxBullets = Defines::Int("Player1_MaxBullets");
    controller.respawnTime = Defines::Float("Player1_RespawnTime");

    pause = std::make_shared<Engine::InputActionButton>("Pause");
    pause->AddGamepadBinding({GLFW_JOYSTICK_1, GLFW_GAMEPAD_BUTTON_START, Engine::GamepadInputID::InputType::Button});
    pause->AddGamepadBinding({GLFW_JOYSTICK_1, GLFW_GAMEPAD_BUTTON_BACK, Engine::GamepadInputID::InputType::Button});
    pause->AddOnStart(nullptr, PauseGame);
    Engine::Systems::inputSystem->Add(pause);

    ecsSystem->AddComponent<Engine::BoxCollider>(player, Engine::BoxCollider());
    ecsSystem->GetComponent<Engine::BoxCollider>(player).size = glm::vec3(0.9f);
    ecsSystem->GetComponent<Engine::BoxCollider>(player).layer = static_cast<unsigned char>(CollisionLayer::Player);
    ecsSystem->AddComponent<Health>(player, Health{Defines::Int("Player1_Health")});
    players.first = player;

    if(glfwJoystickPresent(GLFW_JOYSTICK_2))
    {
        Engine::Entity player2Text = ecsSystem->CreateEntity();
        entities.push_back(player2Text);
        auto &player2UI = ecsSystem->AddComponent<Engine::Text>(player2Text);
        player2UI.scale = 4;
        player2UI.position = {1400, 0};
        player2UI.SetText("Player 2");

        Engine::Entity playerUI2 = ecsSystem->CreateEntity();
        entities.push_back(playerUI2);
        auto &textUI2 = ecsSystem->AddComponent<Engine::Text>(playerUI2);
        textUI2.scale = 4;
        textUI2.position = {1530, 80};
        textUI2.horizontalAlignment = Engine::Text::Center;


        Engine::Entity player2 = Engine::ImportGLTF(Engine::Files::ASSETS / "Graphics\\Models\\Player2.glb")[0];
        entities.push_back(player2);
        ecsSystem->GetComponent<Engine::Name>(player2) = "Player 2";
        PlayerController &controller2 = ecsSystem->AddComponent<PlayerController>(player2);
        controller2.uiTextScore = playerUI2;
        controller2.AddMovementInput(GLFW_JOYSTICK_2, GLFW_GAMEPAD_AXIS_LEFT_X, GLFW_GAMEPAD_AXIS_LEFT_Y);
        controller2.AddMovementInput(GLFW_JOYSTICK_2, GLFW_GAMEPAD_BUTTON_DPAD_LEFT, GLFW_GAMEPAD_BUTTON_DPAD_RIGHT,GLFW_GAMEPAD_BUTTON_DPAD_UP, GLFW_GAMEPAD_BUTTON_DPAD_DOWN);
        controller2.AddFireInput({GLFW_JOYSTICK_2, GLFW_GAMEPAD_BUTTON_A, Engine::GamepadInputID::Button});
        controller2.AddFireInput({GLFW_JOYSTICK_2, GLFW_GAMEPAD_BUTTON_B, Engine::GamepadInputID::Button});
        controller2.AddReviveInput({GLFW_JOYSTICK_2, GLFW_GAMEPAD_BUTTON_X, Engine::GamepadInputID::Button});
        controller2.AddReviveInput({GLFW_JOYSTICK_2, GLFW_GAMEPAD_BUTTON_Y, Engine::GamepadInputID::Button});
        controller2.speed = Defines::Float("Player2_Speed");
        controller2.stunnedTime = Defines::Float("Player2_StunnedTime");
        controller2.bulletSpeed = Defines::Float("Player2_BulletSpeed");
        controller2.maxBullets = Defines::Int("Player2_MaxBullets");
        controller2.respawnTime = Defines::Float("Player2_RespawnTime");

        pause->AddGamepadBinding({GLFW_JOYSTICK_2, GLFW_GAMEPAD_BUTTON_START, Engine::GamepadInputID::InputType::Button});
        pause->AddGamepadBinding({GLFW_JOYSTICK_2, GLFW_GAMEPAD_BUTTON_BACK, Engine::GamepadInputID::InputType::Button});
        Engine::Systems::inputSystem->Remove(pause);
        Engine::Systems::inputSystem->Add(pause);

        ecsSystem->AddComponent<Engine::BoxCollider>(player2, Engine::BoxCollider());
        ecsSystem->GetComponent<Engine::BoxCollider>(player2).size = glm::vec3(0.9f);
        ecsSystem->GetComponent<Engine::BoxCollider>(player2).layer = static_cast<unsigned char>(CollisionLayer::Player);
        ecsSystem->AddComponent<Health>(player2, Health{Defines::Int("Player2_Health")});
        players.second = player2;
    }

    Engine::Systems::renderSystem->camera.SetTranslation(glm::vec3(0,1,-14));
    Engine::Systems::renderSystem->camera.SetScale(glm::vec3(1));
    Engine::Systems::renderSystem->camera.SetRotation(glm::vec3(glm::radians(-12.0f),0,0));

    Systems::dungeonSystem->Initialize();
}

void Game::PauseGame(void*)
{
    if(Engine::GetIsGamePaused())
    {
        Engine::ContinueGame();
        ecsSystem->GetComponent<Engine::Text>(pauseText).scale = 0;
    }
    else
    {
        Engine::PauseGame();
        pauseStartTime = Engine::Systems::timeManager->GetTimeSinceStartupWithoutPauses();
    }
}

void Game::Update(float deltaTime)
{
    Systems::playerControllerSystem->Update(deltaTime);
    Systems::hubertusSystem->Update(deltaTime);
    Systems::kindredSpiritSystem->Update(deltaTime);
    Systems::assiSystem->Update(deltaTime);
    Systems::cuballSystem->Update(deltaTime);
    Systems::dukeSystem->Update(deltaTime);
    Systems::dungeonSystem->Update();
    Systems::elevatorSystem->Update();
    Systems::bulletSystem->Update(deltaTime);
    Systems::destroyerSystem->Update();

    //Number of components needs to be 1 for lootie because the prefab will always exist
    if(Systems::dungeonSystem->IsDungeonCleared() && ecsSystem->GetNumberOfComponents<Loot>() == 1)
        Systems::dungeonSystem->LoadNextDungeon();
}

void Game::UpdateWithoutPause()
{
    if(!Engine::GetIsGamePaused()) return;
    float delta = Engine::Systems::timeManager->GetTimeSinceStartupWithoutPauses() - pauseStartTime;
    if(((int)delta) % 2 == 0)
    {
        Engine::Text& text = ecsSystem->GetComponent<Engine::Text>(pauseText);
        text.scale = pauseTextScale;
    }
    else
    {
        Engine::Text& text = ecsSystem->GetComponent<Engine::Text>(pauseText);
        text.scale = 0;
    }
}

void Game::End()
{
    if(players.first != Engine::Entity::INVALID_ENTITY_ID)
    {
        ecsSystem->GetComponent<PlayerController>(players.first).ResetInput();
        players.first = Engine::Entity::INVALID_ENTITY_ID;
    }

    if(players.second != Engine::Entity::INVALID_ENTITY_ID)
    {
        ecsSystem->GetComponent<PlayerController>(players.second).ResetInput();
        players.second = Engine::Entity::INVALID_ENTITY_ID;
    }

    while(!entities.empty())
    {
        ecsSystem->RemoveEntity(entities.back());
        entities.pop_back();
    }

    //Hier weitermachen
    //Als letztes hattest du in PlayerController einen destructor hinzugefügt
}

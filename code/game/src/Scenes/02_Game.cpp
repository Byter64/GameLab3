#include "Scenes/02_Game.h"
#include "GameDefines.h"
#include "ECSExtension.h"

extern std::pair<Engine::Entity, Engine::Entity> players;
extern int windowWidth, windowHeight;

GLuint litVertexShader;
GLuint litFragmentShader;

void Game::OnStart()
{
    pauseText = CreateEntity();
    Engine::Text& pauseTextText = ecsSystem->AddComponent<Engine::Text>(pauseText);
    pauseTextText.scale = 0;
    pauseTextText.position = {windowWidth / 2, windowHeight / 2};
    pauseTextText.horizontalAlignment = Engine::Text::Center;
    pauseTextText.verticalAlignment = Engine::Text::Center;
    pauseTextText.SetText("Game Paused");

    Engine::Entity playersText = CreateEntity();
    auto& playersUI = ecsSystem->AddComponent<Engine::Text>(playersText);
    playersUI.scale = 4;
    playersUI.position = {890, 0};
    playersUI.SetText("Both");

    Engine::Entity playersUIActualScore = CreateEntity();
    auto& playersTextUI = ecsSystem->AddComponent<Engine::Text>(playersUIActualScore);
    playersTextUI.scale = 4;
    playersTextUI.position = {960, 80};
    playersTextUI.horizontalAlignment = Engine::Text::Center;
    Systems::playerControllerSystem->scoreUI = playersUIActualScore;


    Engine::Entity player1Text = CreateEntity();
    auto& player1UI = ecsSystem->AddComponent<Engine::Text>(player1Text);
    player1UI.scale = 4;
    player1UI.position = {200, 0};
    player1UI.SetText("Player 1");

    Engine::Entity playerUI = CreateEntity();
    auto& textUI = ecsSystem->AddComponent<Engine::Text>(playerUI);
    textUI.scale = 4;
    textUI.position = {330, 80};
    textUI.horizontalAlignment = Engine::Text::Center;


    Engine::Entity player = CreateEntity(Engine::Files::ASSETS / "Graphics\\Models\\Player.glb")[0];
    ecsSystem->GetComponent<Engine::Name>(player) = "Player 1";
    ecsSystem->GetComponent<Engine::Transform>(player).SetRotation(glm::quat(glm::vec3(glm::radians(90.0f),0,0)));
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
    pause->AddOnStart(this, PauseGame);
    Engine::Systems::inputSystem->Add(pause);

    ecsSystem->AddComponent<Engine::BoxCollider>(player, Engine::BoxCollider());
    ecsSystem->GetComponent<Engine::BoxCollider>(player).size = glm::vec3(0.6f);
    ecsSystem->GetComponent<Engine::BoxCollider>(player).layer = static_cast<unsigned char>(CollisionLayer::Player);
    ecsSystem->AddComponent<Health>(player, Health{Defines::Int("Player1_Health"), Defines::Int("Player1_Health")});
    players.first = player;

    if(glfwJoystickPresent(GLFW_JOYSTICK_2))
    {
        Engine::Entity player2Text = CreateEntity();
        auto &player2UI = ecsSystem->AddComponent<Engine::Text>(player2Text);
        player2UI.scale = 4;
        player2UI.position = {1400, 0};
        player2UI.SetText("Player 2");

        Engine::Entity playerUI2 = CreateEntity();
        auto &textUI2 = ecsSystem->AddComponent<Engine::Text>(playerUI2);
        textUI2.scale = 4;
        textUI2.position = {1530, 80};
        textUI2.horizontalAlignment = Engine::Text::Center;


        Engine::Entity player2 = CreateEntity(Engine::Files::ASSETS / "Graphics\\Models\\Player2.glb")[0];
        ecsSystem->GetComponent<Engine::Name>(player2) = "Player 2";
        ecsSystem->GetComponent<Engine::Transform>(player2).SetRotation(glm::quat(glm::vec3(glm::radians(90.0f),0,0)));
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
        ecsSystem->AddComponent<Health>(player2, Health{Defines::Int("Player2_Health"), Defines::Int("Player2_Health")});
        players.second = player2;
    }

    litVertexShader = Engine::Systems::renderSystem->LoadShader(Engine::Files::ASSETS / "Shaders/Lit_VertexAttribute/VS_Lit.vert", GL_VERTEX_SHADER);
    litFragmentShader = Engine::Systems::renderSystem->LoadShader(Engine::Files::ASSETS / "Shaders/Lit_VertexAttribute/FS_Lit.frag", GL_FRAGMENT_SHADER);

    Engine::Systems::renderSystem->camera.SetTranslation(glm::vec3(0,0,-30));
    Engine::Systems::renderSystem->camera.SetScale(glm::vec3(1));
    Engine::Systems::renderSystem->camera.SetRotation(glm::vec3(glm::radians(-30.0f),0,0));

    Systems::dungeonSystem->Initialize();
    Game::scoreP1 = -1;
    Game::scoreP2 = -1;

    /*
    auto test = Engine::ImportGLTF(Engine::Files::ASSETS/ "Graphics\\Models\\Jewel_new.glb")[0];
    ecsSystem->GetComponent<Engine::Transform>(test).SetRotation(glm::quat(glm::vec3(glm::radians(90.0f),glm::radians(-60.0f),glm::radians(-90.0f))));
    ecsSystem->GetComponent<Engine::Transform>(test).SetScale(glm::vec3(1.0f));
    ecsSystem->GetComponent<Engine::Transform>(test).SetTranslation({0, -14, 23});

    for(auto& renderer : Engine::GetComponentsInChildren<Engine::MeshRenderer>(test))
        renderer->SetVertexShader(litVertexShader);
    for(auto& renderer : Engine::GetComponentsInChildren<Engine::MeshRenderer>(test))
        renderer->SetFragmentShader(litFragmentShader);
        */
}

void Game::PauseGame(void* game)
{
    Game* actualGame = (Game*)game;
    if(Engine::GetIsGamePaused())
    {
        Engine::ContinueGame();
        ecsSystem->GetComponent<Engine::Text>(actualGame->pauseText).scale = 0;
    }
    else
    {
        Engine::PauseGame();
        actualGame->pauseStartTime = Engine::Systems::timeManager->GetTimeSinceStartupWithoutPauses();
    }
}

void Game::OnUpdate(float deltaTime)
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

void Game::OnUpdateWithoutPause()
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

void Game::OnEnd()
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

    Engine::Systems::inputSystem->Remove(pause);
}


Engine::Entity Game::CreateWall(glm::vec3 position)
{
    if(!ecsSystem->IsEntityActive(wallPrefab))
    {
        wallPrefab = Engine::ImportGLTF(Engine::Files::ASSETS / "Graphics\\Models\\Wall.glb")[0];
        ecsSystem->GetComponent<Engine::Transform>(wallPrefab).SetScale(glm::vec3(0.0f));
        ecsSystem->GetComponent<Engine::Transform>(wallPrefab).SetRotation(glm::quat(glm::vec3(glm::radians(90.0f),0,0)));
        ecsSystem->GetComponent<Engine::MeshRenderer>(wallPrefab).primitiveData[0].material.baseColorFactor *= 0.7f;
    }

    Engine::Entity wall = ECSHelper::CopyEntity(wallPrefab);
    AddEntity(wall);

    ecsSystem->GetComponent<Engine::Transform>(wall).SetTranslation(position);
    ecsSystem->GetComponent<Engine::Transform>(wall).SetScale(glm::vec3(1.0f));

    return wall;
}

Engine::Entity Game::CreateBreakableWall(glm::vec3 position)
{
    if(!ecsSystem->IsEntityActive(breakableWallPrefab))
    {
        breakableWallPrefab = Engine::ImportGLTF(Engine::Files::ASSETS / "Graphics\\Models\\breakableWall.glb")[0];
        ecsSystem->AddComponent<Health>(breakableWallPrefab, {10, 10});
        ecsSystem->GetComponent<Engine::Transform>(breakableWallPrefab).SetScale(glm::vec3(0.0f));
    }

    Engine::Entity wall = ECSHelper::CopyEntity(breakableWallPrefab);
    AddEntity(wall);

    ecsSystem->GetComponent<Engine::Transform>(wall).SetTranslation(position);
    ecsSystem->GetComponent<Engine::Transform>(wall).SetScale(glm::vec3(1.0f));
    ecsSystem->AddComponent<BreakableWall>(wall);

    Engine::BoxCollider& collider = ecsSystem->AddComponent<Engine::BoxCollider>(wall);
    collider.isStatic = true;
    collider.size = glm::vec3(1);
    collider.layer = (int)CollisionLayer::Dungeon;

    return wall;
}

Engine::Entity Game::CreateBullet(Engine::Entity spawner, glm::vec3 position, glm::vec3 direction, float speed)
{
    if(!ecsSystem->IsEntityActive(bulletPrefab))
    {
        bulletPrefab = Engine::ImportGLTF(Engine::Files::ASSETS / "Graphics\\Models\\Bullet.glb")[0];
        ecsSystem->GetComponent<Engine::Transform>(bulletPrefab).SetScale(glm::vec3(0.0f));
    }

    Engine::Entity entity = ECSHelper::CopyEntity(bulletPrefab);
    AddEntity(entity);

    ecsSystem->GetComponent<Engine::Transform>(entity).SetScale(glm::vec3(0.2f));
    ecsSystem->GetComponent<Engine::Transform>(entity).SetTranslation(position);
    ecsSystem->GetComponent<Engine::Transform>(entity).SetRotation(glm::quat(glm::vec3(glm::radians(90.0f), 0, glm::atan(direction.y, direction.x) + glm::radians(90.0f))));

    Bullet& bullet = ecsSystem->AddComponent<Bullet>(entity);
    bullet.velocity = glm::normalize(direction) * speed;
    bullet.spawner = spawner;

    Engine::BoxCollider& collider = ecsSystem->AddComponent<Engine::BoxCollider>(entity);
    collider.size = glm::vec3(1, 1, 1);
    collider.isStatic = false;
    collider.layer = static_cast<unsigned char>(CollisionLayer::Bullet);
    collider.collisions.clear();

    return entity;
}

Engine::Entity Game::CreateLoot(glm::vec3 position, int points)
{
    if(!ecsSystem->IsEntityActive(lootPrefab))
    {
        lootPrefab = Engine::ImportGLTF(Engine::Files::ASSETS / "Graphics\\Models\\Jewel_new.glb", "Loot_")[0];
        ecsSystem->GetComponent<Engine::Transform>(lootPrefab).SetScale(glm::vec3(0));
        ecsSystem->GetComponent<Engine::Transform>(lootPrefab).SetRotation(glm::quat(glm::vec3(glm::radians(22.5f),0,0)));
        ecsSystem->AddComponent<Loot>(lootPrefab, {0});
    }

    Engine::Entity loot = ECSHelper::CopyEntity(lootPrefab);
    AddEntity(loot);

    ecsSystem->GetComponent<Engine::Transform>(loot).SetScale(glm::vec3(1));
    ecsSystem->GetComponent<Engine::Transform>(loot).SetTranslation(position);
    ecsSystem->GetComponent<Loot>(loot).score = points;

    Engine::BoxCollider& collider = ecsSystem->AddComponent<Engine::BoxCollider>(loot);
    collider.size = glm::vec3(1,1, 1000);
    collider.position = glm::vec3 (0);
    collider.isStatic = true;
    collider.layer = static_cast<unsigned char>(CollisionLayer::Collectible);

    Engine::Systems::animationSystem->PlayAnimation(loot, "Loot_Hovering", true,0, 1);

    return loot;
}


Engine::Entity Game::CreateElevator(glm::vec3 position, Engine::Entity spawnedEnemy)
{
    if(!ecsSystem->IsEntityActive(elevatorPrefab))
    {
        elevatorPrefab = Engine::ImportGLTF(Engine::Files::ASSETS/ "Graphics\\Models\\Elevator.glb", "Elevator_")[0];
        ecsSystem->GetComponent<Engine::Transform>(elevatorPrefab).SetRotation(glm::quat(glm::vec3(glm::radians(90.0f),0,glm::radians(-90.0f))));
        ecsSystem->GetComponent<Engine::Transform>(elevatorPrefab).SetScale(glm::vec3(0.0f));
    }
    Engine::Entity elevator = ECSHelper::CopyEntity(elevatorPrefab, true);
    AddEntity(elevator);

    ecsSystem->GetComponent<Engine::Transform>(elevator).SetScale(glm::vec3(1.0f));
    ecsSystem->GetComponent<Engine::Transform>(elevator).SetTranslation(position);
    ecsSystem->AddComponent<Elevator>(elevator);
    ecsSystem->GetComponent<Elevator>(elevator).spawnedEnemy = spawnedEnemy;
    return elevator;
}

Engine::Entity Game::CreateHubertus(std::pair<int, int> startPos)
{
    if(!ecsSystem->IsEntityActive(hubertusPrefab))
    {
        hubertusPrefab = Engine::ImportGLTF(Engine::Files::ASSETS/ "Graphics\\Models\\Hubertus.glb")[0];
        ecsSystem->GetComponent<Engine::Transform>(hubertusPrefab).SetRotation(glm::quat(glm::vec3(glm::radians(90.0f),0,0)));
        ecsSystem->GetComponent<Engine::Transform>(hubertusPrefab).SetScale(glm::vec3(0.0f));

        for(auto& renderer : Engine::GetComponentsInChildren<Engine::MeshRenderer>(hubertusPrefab))
            renderer->SetVertexShader(litVertexShader);
        for(auto& renderer : Engine::GetComponentsInChildren<Engine::MeshRenderer>(hubertusPrefab))
            renderer->SetFragmentShader(litFragmentShader);
    }
    Engine::Entity enemy = ECSHelper::CopyEntity(hubertusPrefab, true);
    AddEntity(enemy);

    ecsSystem->GetComponent<Engine::Transform>(enemy).SetScale(glm::vec3(1.0f));
    ecsSystem->AddComponent<Engine::BoxCollider>(enemy, Engine::BoxCollider());
    ecsSystem->GetComponent<Engine::BoxCollider>(enemy).size = glm::vec3(0.9f);
    ecsSystem->GetComponent<Engine::BoxCollider>(enemy).layer = static_cast<unsigned char>(CollisionLayer::Enemy);
    EnemyBehaviour behaviour;
    behaviour.spawnTime = Engine::Systems::timeManager->GetTimeSinceStartup();
    ecsSystem->AddComponent<EnemyBehaviour>(enemy, behaviour);
    Hubertus hubertus;
    hubertus.startPos = startPos;
    hubertus.speed = Defines::Float("Hubertus_Speed");
    hubertus.bulletSpeed = Defines::Float("Hubertus_BulletSpeed");
    ecsSystem->AddComponent(enemy, hubertus);
    ecsSystem->AddComponent<Health>(enemy, Health{Defines::Int("Hubertus_Health"), Defines::Int("Hubertus_Health")});

    return enemy;
}

//This function will spawn one at the given position and one at startPos * -1
std::pair<Engine::Entity, Engine::Entity> Game::CreateKindredSpirit(std::pair<int, int> startPos)
{
    if(!ecsSystem->IsEntityActive(kindredSpiritPrefab))
    {
        kindredSpiritPrefab = Engine::ImportGLTF(Engine::Files::ASSETS / "Graphics\\Models\\KindredSpirits.glb")[0];
        ecsSystem->GetComponent<Engine::Transform>(kindredSpiritPrefab).SetRotation(glm::quat(glm::vec3(glm::radians(90.0f), 0, 0)));
        ecsSystem->GetComponent<Engine::Transform>(kindredSpiritPrefab).SetScale(glm::vec3(0.0f));

        for(auto& renderer : Engine::GetComponentsInChildren<Engine::MeshRenderer>(kindredSpiritPrefab))
            renderer->SetVertexShader(litVertexShader);
        for(auto& renderer : Engine::GetComponentsInChildren<Engine::MeshRenderer>(kindredSpiritPrefab))
            renderer->SetFragmentShader(Engine::Systems::renderSystem->LoadShader(Engine::Files::ASSETS / "Shaders/Lit_VertexAttribute/FS_Lit_NoAlpha.frag", GL_FRAGMENT_SHADER));
    }

    Engine::Entity enemy1 = ECSHelper::CopyEntity(kindredSpiritPrefab, true);
    AddEntity(enemy1);
    ecsSystem->GetComponent<Engine::Transform>(enemy1).SetScale(glm::vec3(1.0f));

    ecsSystem->AddComponent<Engine::BoxCollider>(enemy1, Engine::BoxCollider());
    Engine::BoxCollider& coll1 = ecsSystem->GetComponent<Engine::BoxCollider>(enemy1);
    coll1.size = glm::vec3(0.5f);
    coll1.layer = static_cast<unsigned char>(CollisionLayer::Enemy);

    EnemyBehaviour behav1;
    behav1.spawnTime = Engine::Systems::timeManager->GetTimeSinceStartup();
    ecsSystem->AddComponent(enemy1, behav1);
    KindredSpirit kindredSpirit1;
    kindredSpirit1 = KindredSpirit();
    kindredSpirit1.startPos = startPos;
    kindredSpirit1.isMainEntity = true;
    kindredSpirit1.speed = Defines::Float("KindredSpirit_Speed");
    glm::vec4 colour = KindredSpirit::colours.front();
    kindredSpirit1.colour = colour;
    KindredSpirit::colours.pop();
    ecsSystem->AddComponent<Health>(enemy1, Health{Defines::Int("KindredSpirit_Health"), Defines::Int("KindredSpirit_Health")});

    Engine::Entity enemy2 = CopyEntity(kindredSpiritPrefab, true);
    AddEntity(enemy2);
    ecsSystem->GetComponent<Engine::Transform>(enemy2).SetScale(glm::vec3(1.0f));

    ecsSystem->AddComponent<Engine::BoxCollider>(enemy2, Engine::BoxCollider());
    Engine::BoxCollider& coll2 = ecsSystem->GetComponent<Engine::BoxCollider>(enemy2);
    coll2.size = glm::vec3(0.5f);
    coll2.layer = static_cast<unsigned char>(CollisionLayer::Enemy);

    EnemyBehaviour behav2;
    behav2.spawnTime = Engine::Systems::timeManager->GetTimeSinceStartup();
    KindredSpirit kindredSpirit2;
    ecsSystem->AddComponent(enemy2, behav2);
    kindredSpirit2 = KindredSpirit();
    kindredSpirit2.startPos = startPos; //this value needs to be the same as for the first spirit
    kindredSpirit2.speed = Defines::Float("KindredSpirit_Speed");
    kindredSpirit2.isMainEntity = false;
    kindredSpirit2.other = enemy1;
    ecsSystem->AddComponent(enemy2, Health{Defines::Int("KindredSpirit_Health"), Defines::Int("KindredSpirit_Health")});

    kindredSpirit1.other = enemy2;
    ecsSystem->AddComponent(enemy1, kindredSpirit1);
    ecsSystem->AddComponent(enemy2, kindredSpirit2);


    for(Engine::MeshRenderer* renderer : Engine::GetComponentsInChildren<Engine::MeshRenderer>(enemy1))
        for(Engine::MeshRenderer::PrimitiveData &primitive : renderer->primitiveData)
            primitive.material.baseColorFactor = colour;
    for(Engine::MeshRenderer* renderer : Engine::GetComponentsInChildren<Engine::MeshRenderer>(enemy2))
        for(Engine::MeshRenderer::PrimitiveData &primitive : renderer->primitiveData)
            primitive.material.baseColorFactor = colour;

    return {enemy1, enemy2};
}


Engine::Entity Game::CreateAssi(std::pair<int, int> startPos)
{
    if(!ecsSystem->IsEntityActive(assiPrefab))
    {
        assiPrefab = Engine::ImportGLTF(Engine::Files::ASSETS/ "Graphics\\Models\\Assi.glb")[0];
        ecsSystem->GetComponent<Engine::Transform>(assiPrefab).SetRotation(glm::quat(glm::vec3(glm::radians(90.0f),0,0)));
        ecsSystem->GetComponent<Engine::Transform>(assiPrefab).SetScale(glm::vec3(0.0f));

        for(auto& renderer : Engine::GetComponentsInChildren<Engine::MeshRenderer>(assiPrefab))
            renderer->SetVertexShader(litVertexShader);
        for(auto& renderer : Engine::GetComponentsInChildren<Engine::MeshRenderer>(assiPrefab))
            renderer->SetFragmentShader(litFragmentShader);
    }
    Engine::Entity enemy = ECSHelper::CopyEntity(assiPrefab, true);
    AddEntity(enemy);

    ecsSystem->GetComponent<Engine::Transform>(enemy).SetScale(glm::vec3(1.0f));
    ecsSystem->AddComponent<Engine::BoxCollider>(enemy, Engine::BoxCollider());
    ecsSystem->GetComponent<Engine::BoxCollider>(enemy).size = glm::vec3(0.5f);
    ecsSystem->GetComponent<Engine::BoxCollider>(enemy).layer = static_cast<unsigned char>(CollisionLayer::Enemy);
    EnemyBehaviour behaviour;
    behaviour.spawnTime = Engine::Systems::timeManager->GetTimeSinceStartup();
    ecsSystem->AddComponent(enemy, behaviour);
    Assi assi;
    assi = Assi();
    assi.startPos = startPos;
    assi.speed = Defines::Float("Assi_Speed");
    ecsSystem->AddComponent(enemy, assi);
    ecsSystem->AddComponent<Health>(enemy, Health{Defines::Int("Assi_Health"), Defines::Int("Assi_Health")});

    return enemy;
}


Engine::Entity Game::CreateCuball(std::pair<int, int> startPos)
{
    if(!ecsSystem->IsEntityActive(cuballPrefab))
    {
        cuballPrefab = Engine::ImportGLTF(Engine::Files::ASSETS/ "Graphics\\Models\\Cuball.glb", "Cuball_")[0];
        Engine::Transform& transform = ecsSystem->GetComponent<Engine::Transform>(cuballPrefab);
        transform.SetRotation(glm::quat(glm::vec3(glm::radians(90.0f),0,0)));
        transform.SetScale(glm::vec3(0.0f));
        ecsSystem->GetComponent<Engine::MeshRenderer>(ecsSystem->GetEntity(*transform.GetChild(0))).isActive = false;
        for(auto& renderer : Engine::GetComponentsInChildren<Engine::MeshRenderer>(ecsSystem->GetEntity(*transform.GetChild(2))))
            renderer->isActive = false;
    }
    Engine::Entity enemy = ECSHelper::CopyEntity(cuballPrefab, true);
    AddEntity(enemy);

    ecsSystem->GetComponent<Engine::Transform>(enemy).SetScale(glm::vec3(1.0f));
    ecsSystem->AddComponent<Engine::BoxCollider>(enemy, Engine::BoxCollider());
    ecsSystem->GetComponent<Engine::BoxCollider>(enemy).size = glm::vec3(0.5f);
    ecsSystem->GetComponent<Engine::BoxCollider>(enemy).layer = static_cast<unsigned char>(CollisionLayer::Enemy);
    EnemyBehaviour behaviour;
    behaviour.spawnTime = Engine::Systems::timeManager->GetTimeSinceStartup();
    ecsSystem->AddComponent<EnemyBehaviour>(enemy, behaviour);
    Cuball cuball;
    cuball = Cuball{};
    cuball.startPos = startPos;
    cuball.speed = Defines::Float("Cuball_Cube_Speed");
    cuball.bulletSpeed = Defines::Float("Cuball_Cube_BulletSpeed");
    cuball.rotationParent = ecsSystem->CreateEntity();
    ecsSystem->AddComponent(enemy, cuball);
    ecsSystem->AddComponent<Engine::Transform>(cuball.rotationParent);
    ecsSystem->AddComponent<Health>(enemy, Health{Defines::Int("Cuball_Cube_Health"), Defines::Int("Cuball_Cube_Health")});

    return enemy;
}

Engine::Entity Game::CreateDuke(std::pair<int, int> startPos)
{
    if(!ecsSystem->IsEntityActive(dukePrefab))
    {
        dukePrefab = Engine::ImportGLTF(Engine::Files::ASSETS/ "Graphics\\Models\\Duke.glb")[0];
        ecsSystem->GetComponent<Engine::Transform>(dukePrefab).SetRotation(glm::quat(glm::vec3(glm::radians(90.0f),0,0)));
        ecsSystem->GetComponent<Engine::Transform>(dukePrefab).SetScale(glm::vec3(0.0f));
    }
    Engine::Entity enemy = ECSHelper::CopyEntity(dukePrefab, true);
    AddEntity(enemy);

    ecsSystem->GetComponent<Engine::Transform>(enemy).SetScale(glm::vec3(1.0f));
    ecsSystem->AddComponent<Engine::BoxCollider>(enemy, Engine::BoxCollider());
    ecsSystem->GetComponent<Engine::BoxCollider>(enemy).size = glm::vec3(1.0f);
    ecsSystem->GetComponent<Engine::BoxCollider>(enemy).layer = static_cast<unsigned char>(CollisionLayer::Enemy);
    EnemyBehaviour behaviour;
    Duke duke;
    duke.speed = Defines::Float("Duke_Speed");
    duke.bulletSpeed = Defines::Float("Duke_BulletSpeed");
    duke.startPos = startPos;
    ecsSystem->AddComponent(enemy, duke);
    behaviour.spawnTime = Engine::Systems::timeManager->GetTimeSinceStartup();
    ecsSystem->AddComponent<EnemyBehaviour>(enemy, behaviour);
    ecsSystem->AddComponent<Health>(enemy, Health{Defines::Int("Duke_Health"), Defines::Int("Duke_Health")});

    return enemy;
}

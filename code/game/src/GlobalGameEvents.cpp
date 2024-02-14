#include "GlobalGameEvents.h"
#include "Engine.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

extern Engine::InputSystem* inputSystem;

extern std::shared_ptr<Engine::RenderSystem> renderSystem; //Never change this name, as Systems depend on this symbol being declared somewhere!!!!!!!!!!!!!!!?!?!?!?!"?!?§!"$
extern std::shared_ptr<Engine::CollisionSystem> collisionSystem; //Never change this name, as Systems depend on this symbol being declared somewhere!!!!!!!!!!!!!!!?!?!?!?!"?!?§!"$
extern std::shared_ptr<Engine::EnemyBehaviourSystem> enemyBehaviourSystem; //Never change this name, as Systems depend on this symbol being declared somewhere!!!!!!!!!!!!!!!?!?!?!?!"?!?§!"$
extern std::shared_ptr<Engine::DungeonSystem> dungeonSystem;
extern GLFWwindow *window;

Engine::Entity krawatterich;
Engine::Entity root;

void Engine::OnStartGame()
{
    inputSystem = new Engine::InputSystem(window);

    Engine::Entity dungeon = ecsSystem->CreateEntity();
    ecsSystem->AddComponent<Engine::Name>(dungeon, "Dungeon");
    ecsSystem->AddComponent(dungeon, Engine::Transform());
    ecsSystem->AddComponent<Engine::Dungeon>(dungeon, Engine::Dungeon(Engine::Files::ASSETS / "Dungeons", "Dungeon_"));

    enemyBehaviourSystem->Initialize(dungeonSystem->wallMap);

    Engine::Entity player = Engine::ImportGLTF(Engine::Files::ASSETS / "Graphics\\Models\\Player\\Player.glb")[0];
    Engine::PlayerController& controller = ecsSystem->AddComponent<Engine::PlayerController>(player);
    controller.speed = 2;
    controller.SetMovementInput(GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_Q, GLFW_KEY_E);
    controller.SetFireInput(GLFW_KEY_SPACE);
    ecsSystem->AddComponent<Engine::BoxCollider>(player, Engine::BoxCollider());
    ecsSystem->GetComponent<Engine::BoxCollider>(player).size = glm::vec3(0.9f);
    ecsSystem->AddComponent<Engine::Health>(player, Engine::Health{1});

    Engine::Entity enemy = Engine::ImportGLTF(Engine::Files::ASSETS/ "Graphics\\Models\\Hubertus\\Hubertus.glb")[0];
    ecsSystem->GetComponent<Engine::Transform>(enemy).SetRotation(glm::quat(glm::vec3(glm::radians(90.0f),0,0)));
    ecsSystem->AddComponent<Engine::BoxCollider>(enemy, Engine::BoxCollider());
    ecsSystem->GetComponent<Engine::BoxCollider>(enemy).size = glm::vec3(0.5f);
    ecsSystem->AddComponent<Engine::EnemyBehaviour>(enemy, Engine::EnemyBehaviour());
    ecsSystem->AddComponent<Engine::Health>(enemy, Engine::Health());
    ecsSystem->GetComponent<Engine::Health>(enemy).health = 3;

    renderSystem->camera.SetTranslation(glm::vec3(0,0,-12));
    renderSystem->camera.SetScale(glm::vec3(1));
    renderSystem->camera.SetRotation(glm::vec3(glm::radians(-15.0f),0,0));
}

void Engine::OnEndGame()
{

}

glm::vec3 movement;

void MovementXY(void*, glm::vec2 input)
{
    movement.x = input.x;
    movement.y = input.y;

    if(movement != glm::vec3(0,0,0))
        movement = glm::normalize(movement) * 8.0f;
}

void MovementZ(void*, glm::vec2 input)
{
    movement.z = input.x;

    if(movement != glm::vec3(0,0,0))
        movement = glm::normalize(movement) * 8.0f;
}

void LoadDemo()
{

    std::shared_ptr<Engine::InputActionVec2> movementXY = std::make_shared<Engine::InputActionVec2>("MovementXY");
    movementXY->AddKeyboardBinding(GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_W, GLFW_KEY_S);
    movementXY->AddOnValueChange(nullptr, MovementXY);

    inputSystem->Add(movementXY);


    std::shared_ptr<Engine::InputActionVec2> movementZ = std::make_shared<Engine::InputActionVec2>("MovementZ");
    movementZ->AddKeyboardBinding(GLFW_KEY_E, GLFW_KEY_Q, 0, 0);
    movementZ->AddOnValueChange(nullptr, MovementZ);
    inputSystem->Add(movementZ);

    std::filesystem::path path = "C:/Users/Yanni/Desktop/Fliegengesicht.gltf";
    root = ecsSystem->CreateEntity();
    ecsSystem->AddComponent<Engine::Name>(root, "Root");
    ecsSystem->AddComponent<Engine::Transform>(root, Engine::Transform());

    for(Engine::Entity entity : Engine::ImportGLTF(path))
    {
        ecsSystem->GetComponent<Engine::Transform>(entity).SetParent(&ecsSystem->GetComponent<Engine::Transform>(root));
    }
    krawatterich = Engine::FindChild(root, "Krawatterich");

    Engine::BoxCollider& boxCollider = ecsSystem->AddComponent<Engine::BoxCollider>(krawatterich);

    GLuint vertexColorshader = renderSystem->LoadShader(Engine::Files::ASSETS / "Shaders/Default/FS_Default_VertexColor.frag", GL_FRAGMENT_SHADER);
    Engine::Entity vertexPaintedSphere1 = Engine::FindChild(root, "VertexPaintedSphere");
    ecsSystem->GetComponent<Engine::MeshRenderer>(vertexPaintedSphere1).primitiveData[0].shader.SetFragmentShader
    (vertexColorshader);
    Engine::Entity vertexPaintedSphere2 = Engine::FindChild(root, "VertexPaintedSphere.001");
    ecsSystem->GetComponent<Engine::MeshRenderer>(vertexPaintedSphere2).primitiveData[0].shader.SetFragmentShader
    (vertexColorshader);
}
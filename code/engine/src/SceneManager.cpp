#include "SceneManager.h"
#include "Systems.h"

std::shared_ptr<Engine::SceneManager>& sceneManager = Engine::Systems::sceneManager;

namespace Engine
{
    void SceneManager::Update()
    {
        if(isLoadRequested)
        {
            activeScene->End();
            activeScene.swap(newScene);
            activeScene->OnStart();
            isLoadRequested = false;
        }
    }

    void SceneManager::Initialize()
    {
        activeScene = std::make_unique<Scene>();
    }
} // Engine
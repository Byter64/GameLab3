#include "SceneManager.h"
#include "Systems.h"

std::shared_ptr<Engine::SceneManager>& sceneManager = Engine::Systems::sceneManager;

namespace Engine
{
    Scene &SceneManager::GetActiveScene()
    {
        return *activeScene;
    }
} // Engine
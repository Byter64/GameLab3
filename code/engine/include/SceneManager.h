#pragma once
#include "Scene.h"
#include <string>

namespace Engine
{

    class SceneManager
    {
        std::unique_ptr<Scene> activeScene;

    public:

        /// Unloads the currently active scene and loads the given Scene
        /// \tparam T
        template<class T>
        void LoadScene()
        {
            if (activeScene != nullptr)
                activeScene->End();

            Scene *newScene = dynamic_cast<Scene *>(new T());
            if (newScene == nullptr)
                throw std::runtime_error((std::string) "You are trying to Load a type " + typeid(T).name() + " as scene that does not inherit from scene.");

            activeScene.reset(newScene);
            activeScene->Start();
        }

        Scene& GetActiveScene();
    };

} // Engine

extern std::shared_ptr<Engine::SceneManager>& sceneManager;
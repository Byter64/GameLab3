#pragma once
#include "Scene.h"
#include <string>

namespace Engine
{

    class SceneManager
    {
        std::unique_ptr<Scene> activeScene{};
        std::unique_ptr<Scene> newScene{};
        bool isLoadRequested = false;
    public:
        void Initialize();


        /// Unloads the currently active scene and loads the given Scene
        /// \tparam T
        template<class T>
        void LoadScene()
        {
            Scene *newScene = dynamic_cast<Scene *>(new T());
            if (newScene == nullptr)
                throw std::runtime_error((std::string) "You are trying to Load a type " + typeid(T).name() + " as scene that does not inherit from scene.");

            this->newScene.reset(newScene);
            isLoadRequested = true;
        }

        Scene& GetActiveScene();
        void Update();
    };

} // Engine

extern std::shared_ptr<Engine::SceneManager>& sceneManager;
#pragma once
#include "ECSSystem.h"
#include "filesystem"

//Please never call this
void GameMain();

namespace Engine
{
    class SceneManager;

    class Scene
    {
        friend class SceneManager;
        friend void ::GameMain();
        std::set<Entity> entities{};
        void Start();
        void Update(float deltaTime);
        void UpdateWithoutPause();
        void End();
    public:

        ///Create all entities here that are part of the scene from the beginning
        virtual void OnStart();

        virtual void OnUpdate(float deltaTime);
        virtual void OnUpdateWithoutPause();

        ///Entities will be destroyed automatically after OnEnd()
        virtual void OnEnd();

        Entity CreateEntity();
        void AddEntity(Entity entity);
        std::vector<Entity> CreateEntity(std::filesystem::path path, std::string animationPrefix = "", bool addParent = false);
        void RemoveEntity(Entity entity);
    };

} // Engine

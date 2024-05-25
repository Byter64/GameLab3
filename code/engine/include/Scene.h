#pragma once
#include "ECSSystem.h"

namespace Engine
{
    class SceneManager;

    class Scene
    {
        friend class SceneManager;
        std::set<Entity> entities{};
        void Start();
        void End();
    public:

        ///Create all entities here that are part of the scene from the beginning
        virtual void OnStart() = 0;

        ///Entities will be destroyed automatically after OnEnd()
        virtual void OnEnd() = 0;

        Entity CreateEntity();
        void RemoveEntity(Entity entity);
    };

} // Engine

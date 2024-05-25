#include "Scene.h"

namespace Engine
{
    Entity Scene::CreateEntity()
    {
        Entity entity = ecsSystem->CreateEntity();
        entities.insert(entity);
        return entity;
    }

    void Scene::RemoveEntity(Entity entity)
    {
        entities.erase(entity);
        ecsSystem->RemoveEntity(entity);
    }

    void Scene::Start()
    {
        OnStart();
    }

    void Scene::End()
    {
        OnEnd();

        for(Entity entity : entities)
            ecsSystem->RemoveEntity(entity);
        entities.clear();
    }
} // Engine
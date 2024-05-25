#include "Scene.h"
#include "EntityUtility.h"

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
            if(ecsSystem->IsEntityActive(entity))
                RemoveEntityWithChildren(entity);
        entities.clear();
    }

    std::vector<Entity> Scene::CreateEntity(std::filesystem::path path, std::string animationPrefix, bool addParent)
    {
        std::vector<Entity> entities = ImportGLTF(path, animationPrefix, addParent);
        for(Entity entity : entities)
        {
            this->entities.insert(entity);
        }
        return entities;
    }

    void Scene::Update(float deltaTime)
    {
        OnUpdate(deltaTime);
    }

    void Scene::UpdateWithoutPause()
    {
        OnUpdateWithoutPause();
    }

    void Scene::OnUpdate(float deltaTime)
    {

    }

    void Scene::OnUpdateWithoutPause()
    {

    }

    void Scene::OnStart()
    {

    }

    void Scene::OnEnd()
    {

    }

    void Scene::AddEntity(Entity entity)
    {
        entities.insert(entity);
    }
} // Engine
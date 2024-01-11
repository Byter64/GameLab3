#pragma once
#include "Entity.h"
#include <list>
#include <map>

class GameObjectManager
{
    std::list<GameObject*> purgatory;
    std::map<Engine::Entity, GameObject*> entityToGameObject;
public:
    void AddToPurgatory(GameObject* gameObject);
    void DeletePurgatory();
    void RegisterGameObject(GameObject* gameObject);
};

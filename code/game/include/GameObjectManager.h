#pragma once
#include "GameObject.h"
#include <list>

class GameObjectManager
{
    std::list<GameObject*> purgatory;

public:
    void AddToPurgatory(GameObject* gameObject);
    void DeletePurgatory();
};

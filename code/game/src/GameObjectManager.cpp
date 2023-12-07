#include "GameObjectManager.h"

void GameObjectManager::AddToPurgatory(GameObject *gameObject)
{
    purgatory.push_back(gameObject);
}

void GameObjectManager::DeletePurgatory()
{
    for(GameObject* gameObject : purgatory)
    {
        delete gameObject;
    }
}

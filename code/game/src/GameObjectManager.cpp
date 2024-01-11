#include "GameObjectManager.h"

void GameObjectManager::AddToPurgatory(GameObject *gameObject)
{
    purgatory.push_back(gameObject);
}

void GameObjectManager::DeletePurgatory()
{
    while(!purgatory.empty())
    {
        GameObject* gameObject = *purgatory.begin();
        purgatory.pop_front();
        entityToGameObject.erase(gameObject->entity);
        delete gameObject;
    }
}

void GameObjectManager::RegisterGameObject(GameObject *gameObject)
{
    entityToGameObject[gameObject->entity] = gameObject;
}

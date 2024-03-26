#include "Systems/ElevatorSystem.h"
#include "ECSExtension.h"

void ElevatorSystem::EntityAdded(Engine::Entity entity)
{

}

void ElevatorSystem::EntityRemoved(Engine::Entity entity)
{

}

void ElevatorSystem::Update()
{
    for(Engine::Entity entity : entities)
    {
        Elevator& elevator = ecsSystem->GetComponent<Elevator>(entity);

        if(!ecsSystem->HasComponent<Engine::Animator>(entity))
        {
            EnemyBehaviour& behaviour = ecsSystem->GetComponent<EnemyBehaviour>(elevator.spawnedEnemy);
            behaviour.isActive = true;
            Engine::RemoveEntityWithChildren(entity);
            continue;
        }
        Engine::Transform* elevatorModel = ecsSystem->GetComponent<Engine::Transform>(entity).GetChildren().front();

        //0.8f is the animation height of the elevator
        //2.2f is the time where the elevator is at the height of 0.8f
        if(ecsSystem->GetComponent<Engine::Animator>(entity).currentTime < 2.2f)
            ecsSystem->GetComponent<Engine::Transform>(elevator.spawnedEnemy).SetTranslation(elevatorModel->GetGlobalTranslation() - glm::vec3(0,0,0.8f));
    }
}

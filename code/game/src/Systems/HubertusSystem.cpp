#include "Systems/HubertusSystem.h"
#include "ECSExtension.h"
#include "GameDefines.h"
#include <random>

HubertusSystem::HubertusSystem()
{
    idleDurationRange.first = Defines::Float("Hubertus_IdleDuration_Min");
    idleDurationRange.second = Defines::Float("Hubertus_IdleDuration_Max");
    walkDurationRange.first = Defines::Float("Hubertus_WalkDuration_Min");
    walkDurationRange.second = Defines::Float("Hubertus_WalkDuration_Max");
    shootIntervalRange.first = Defines::Float("Hubertus_ShootInterval_Min");
    shootIntervalRange.second = Defines::Float("Hubertus_ShootInterval_Max");
}

void HubertusSystem::EntityAdded(Engine::Entity entity)
{
    Hubertus &hubertus = ecsSystem->GetComponent<Hubertus>(entity);
    Engine::Transform& transform = ecsSystem->GetComponent<Engine::Transform>(entity);

    transform.SetTranslation(glm::vec3(Systems::dungeonSystem->ToGlobal(hubertus.startPos), 0));

    std::vector<std::pair<int,int>> targetNodes = Systems::enemyBehaviourSystem->FindNodes(hubertus.startPos.first, hubertus.startPos.second);
    std::pair<int, int> target = targetNodes[rand() % targetNodes.size()];
    Systems::enemyBehaviourSystem->SetTarget(hubertus.movement, hubertus.startPos, target);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> distr(shootIntervalRange.first, shootIntervalRange.second);
    hubertus.shootTimer = distr(gen);
    distr = std::uniform_real_distribution<>(walkDurationRange.first, walkDurationRange.second);
    hubertus.idleTimer = distr(gen);
    hubertus.isMoving = true;

}

void HubertusSystem::EntityRemoved(Engine::Entity entity)
{

}

void HubertusSystem::Update(Engine::Entity entity, float deltaTime)
{
    EnemyBehaviour& behaviour = ecsSystem->GetComponent<EnemyBehaviour>(entity);
    Hubertus &hubertus = ecsSystem->GetComponent<Hubertus>(entity);
    Engine::Transform &transform = ecsSystem->GetComponent<Engine::Transform>(entity);

    if(!behaviour.isActive) return;

    std::random_device rd;
    std::mt19937 gen(rd());
    if(hubertus.shootTimer <= 0)
    {
        ECSHelper::SpawnBullet(entity, transform.GetGlobalTranslation(), glm::vec3(hubertus.movement.direction, 0), hubertus.bulletSpeed);
        std::uniform_real_distribution<> distr(shootIntervalRange.first, shootIntervalRange.second);
        hubertus.shootTimer = distr(gen);
    }
    if(hubertus.idleTimer <= 0)
    {
        std::uniform_real_distribution<> distr;
        if(hubertus.isMoving)
            distr = std::uniform_real_distribution<>(idleDurationRange.first, idleDurationRange.second);
        else
            distr = std::uniform_real_distribution<>(walkDurationRange.first, walkDurationRange.second);
        hubertus.isMoving = !hubertus.isMoving;
        hubertus.idleTimer = distr(gen);
    }
    hubertus.idleTimer -= deltaTime;
    hubertus.shootTimer -= deltaTime;

    if(hubertus.isMoving)
    {
        Systems::enemyBehaviourSystem->MoveRandomly(hubertus.movement, deltaTime);
        transform.SetTranslation(glm::vec3(hubertus.movement.currentPos, 0));
    }
}

void HubertusSystem::HandleDamage(Engine::Entity entity, Engine::Entity other)
{
    //Bullet is already destroying itself, so no need to do it here
    Health& health = ecsSystem->GetComponent<Health>(entity);
    EnemyBehaviour& behaviour = ecsSystem->GetComponent<EnemyBehaviour>(entity);

    if(!behaviour.isActive) return;

    health.health--;
    if(health.health <= 0)
    {
        Systems::enemyBehaviourSystem->Kill(entity);
    }
}

void HubertusSystem::Update(float deltaTime)
{
    for (Engine::Entity entity : entities)
    {
        Update(entity, deltaTime);

        for(auto& collision : ecsSystem->GetComponent<Engine::BoxCollider>(entity).collisions)
        {
            Engine::Entity other = collision.other;
            //If is a bullet and was not shot from an enemy
            if(ecsSystem->HasComponent<Bullet>(other) && !ecsSystem->HasComponent<EnemyBehaviour>(ecsSystem->GetComponent<Bullet>(other).spawner))
                HandleDamage(entity, other);
        }
    }
}

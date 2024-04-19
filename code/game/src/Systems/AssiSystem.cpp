#include "Systems/AssiSystem.h"
#include "ECSExtension.h"
#include "GameDefines.h"
#include <random>

AssiSystem::AssiSystem()
{
    idleDurationRange.first = Defines::Float("Assi_IdleDuration_Min");
    idleDurationRange.second = Defines::Float("Assi_IdleDuration_Max");

    Assi::stunnedTime = Defines::Float("Assi_StunnedTime");
}

void AssiSystem::EntityAdded(Engine::Entity entity)
{
    Assi& assi = ecsSystem->GetComponent<Assi>(entity);
    Engine::Transform& transform = ecsSystem->GetComponent<Engine::Transform>(entity);

    transform.SetTranslation(glm::vec3(Systems::dungeonSystem->ToGlobal(assi.startPos), 0));

    std::vector<std::pair<int,int>> targetNodes = Systems::enemyBehaviourSystem->FindNodes(assi.startPos.first, assi.startPos.second);
    std::pair<int, int> target = targetNodes[rand() % targetNodes.size()];
    Systems::enemyBehaviourSystem->SetTarget(assi.movement, assi.startPos, target);
    assi.movement.currentPos = Systems::dungeonSystem->ToGlobal(assi.startPos);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> distr(idleDurationRange.first, idleDurationRange.second);
    assi.idleTimer = distr(gen);
    assi.isMoving = true;
}

void AssiSystem::EntityRemoved(Engine::Entity entity)
{

}

void AssiSystem::Update(Engine::Entity entity, float deltaTime)
{
    EnemyBehaviour &behaviour = ecsSystem->GetComponent<EnemyBehaviour>(entity);
    Assi& assi = ecsSystem->GetComponent<Assi>(entity);
    Engine::Transform &transform = ecsSystem->GetComponent<Engine::Transform>(entity);

    if(!behaviour.isActive) return;

    if(assi.stunnedTimer > 0.0f)
    {
        assi.stunnedTimer -= deltaTime;
        return;
    }

    if(assi.isMoving)
        Move(entity, deltaTime);

    if(assi.idleTimer <= 0)
    {
        std::uniform_real_distribution<> distr;
        if(assi.isMoving)
        {
            std::random_device rd;
            std::mt19937 gen(rd());
            if(assi.isPlayerInSight)
                distr = std::uniform_real_distribution<>(idleDurationRange.first, idleDurationRange.first);
            else
                distr = std::uniform_real_distribution<>(idleDurationRange.first, idleDurationRange.second);
            assi.idleTimer = distr(gen);

            transform.SetTranslation(glm::round(transform.GetTranslation()));
        }
        else
        {
            assi.idleTimer = 1 / assi.speed; //How long it takes to move by one unit
        }
        assi.isMoving = !assi.isMoving;
    }
    assi.idleTimer -= deltaTime;
}

void AssiSystem::HandleDamage(Engine::Entity entity, Engine::Entity other)
{
    //Bullet is already destroying itself, so no need to do it here
    EnemyBehaviour& behaviour = ecsSystem->GetComponent<EnemyBehaviour>(entity);
    Assi& assi = ecsSystem->GetComponent<Assi>(entity);
    Engine::Transform& transform = ecsSystem->GetComponent<Engine::Transform>(entity);
    Engine::Transform& otherTransform = ecsSystem->GetComponent<Engine::Transform>(other);

    if(!behaviour.isActive) return;

    glm::vec2 dir = otherTransform.GetGlobalTranslation() - transform.GetGlobalTranslation();
    dir = Miscellaneous::RoundTo4Directions(glm::vec3(dir, 0));
    glm::quat rot = glm::normalize(glm::quat(glm::vec3(glm::radians(90.0f), 0, glm::atan(dir.y, dir.x))));
    if(assi.stunnedTimer <= 0 || rot == transform.GetRotation())
    {
        assi.stunnedTimer = Assi::stunnedTime;
        transform.SetRotation(rot);
        return;
    }

    Health& health = ecsSystem->GetComponent<Health>(entity);
    health.health--;
    if(health.health <= 0)
    {
        Systems::enemyBehaviourSystem->Kill(entity);
    }
}

void AssiSystem::Move(Engine::Entity entity, float deltaTime)
{
    Assi& assi = ecsSystem->GetComponent<Assi>(entity);
    Engine::Transform &transform = ecsSystem->GetComponent<Engine::Transform>(entity);
    Engine::Entity player = Systems::enemyBehaviourSystem->FindPlayerInSight(entity, INT_MAX);
    if(player != Engine::Entity::INVALID_ENTITY_ID)
    {
        assi.isPlayerInSight = true;
        glm::vec3 direction = ecsSystem->GetComponent<Engine::Transform>(player).GetGlobalTranslation() - transform.GetGlobalTranslation();
        Systems::enemyBehaviourSystem->MoveStraight(assi.movement, direction, assi.speed * deltaTime);
        transform.SetTranslation(glm::vec3(assi.movement.currentPos, 0));

        float angle = glm::atan(direction.y, direction.x);
        angle /= glm::radians(90.0f);
        angle = glm::round(angle);
        angle *= glm::radians(90.0f);
        transform.SetRotation(glm::quat(glm::vec3(glm::radians(90.0f), 0, angle)));
    }
    else if (assi.isPlayerInSight)
    {
        assi.isPlayerInSight = false;

        std::pair<int, int> pos = Systems::dungeonSystem->ToDungeon(glm::round(transform.GetGlobalTranslation()));
        std::vector<std::pair<int,int>> targetNodes = Systems::enemyBehaviourSystem->FindNodes(pos.first, pos.second);
        std::pair<int, int> target = targetNodes[rand() % targetNodes.size()];
        Systems::enemyBehaviourSystem->SetTarget(assi.movement, pos, target);
    }
    else
    {
        Systems::enemyBehaviourSystem->MoveRandomly(assi.movement, assi.speed * deltaTime);
        transform.SetTranslation(glm::vec3(assi.movement.currentPos, 0));
    }
}

void AssiSystem::Update(float deltaTime)
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

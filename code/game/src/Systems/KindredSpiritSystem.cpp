#include "Systems/KindredSpiritSystem.h"
#include "ECSExtension.h"
#include "GameDefines.h"

KindredSpiritSystem::KindredSpiritSystem()
{
    KindredSpirit::maxTimeDifference = Defines::Float("KindredSpirit_TimeTolerance");
    KindredSpirit::colours.push({1.00f, 0.68f, 0.21f, 1.0f});
    KindredSpirit::colours.push({0.27f, 0.17f, 1.00f, 1.0f});
    KindredSpirit::colours.push({0.88f, 0.13f, 0.29f, 1.0f});
    KindredSpirit::colours.push({0.28f, 1.00f, 0.98f, 1.0f});
    KindredSpirit::colours.push({0.63f, 0.28f, 1.00f, 1.0f});
    KindredSpirit::colours.push({0.58f, 1.00f, 0.28f, 1.0f});
    KindredSpirit::colours.push({1.00f, 1.00f, 1.00f, 1.0f});
    KindredSpirit::colours.push({0.40f, 0.40f, 0.40f, 1.0f});
}

void KindredSpiritSystem::EntityAdded(Engine::Entity entity)
{
    KindredSpirit &kindredSpirit = ecsSystem->GetComponent<KindredSpirit>(entity);
    Engine::Transform& transform = ecsSystem->GetComponent<Engine::Transform>(entity);

    if(kindredSpirit.isMainEntity)
    {
        transform.SetTranslation(glm::vec3(Systems::dungeonSystem->ToGlobal(kindredSpirit.startPos), 0));
        std::vector<std::pair<int,int>> targetNodes = Systems::enemyBehaviourSystem->FindNodes(kindredSpirit.startPos.first, kindredSpirit.startPos.second);
        std::pair<int, int> target = targetNodes[rand() % targetNodes.size()];
        Systems::enemyBehaviourSystem->SetTarget(kindredSpirit.movement, kindredSpirit.startPos, target);
        kindredSpirit.movement.currentPos = Systems::dungeonSystem->ToGlobal(kindredSpirit.startPos);
    }
    else
        transform.SetTranslation(glm::vec3(Systems::dungeonSystem->ToGlobal(kindredSpirit.startPos) * -1.0f, 0));
}

void KindredSpiritSystem::EntityRemoved(Engine::Entity entity)
{
    KindredSpirit& kindredSpirit = ecsSystem->GetComponent<KindredSpirit>(entity);
    if(kindredSpirit.isMainEntity)
    {
        glm::vec4 colour = kindredSpirit.colour;
        KindredSpirit::colours.push(colour);
    }
}


void KindredSpiritSystem::Update(Engine::Entity entity, float deltaTime)
{
    EnemyBehaviour &behaviour = ecsSystem->GetComponent<EnemyBehaviour>(entity);
    KindredSpirit& kindredSpirit = ecsSystem->GetComponent<KindredSpirit>(entity);
    Engine::Transform &transform = ecsSystem->GetComponent<Engine::Transform>(entity);

    if(!behaviour.isActive) return;

    if(kindredSpirit.isMainEntity)
    {
        Systems::enemyBehaviourSystem->MoveRandomly(kindredSpirit.movement, kindredSpirit.speed * deltaTime);
        transform.SetTranslation(glm::vec3(kindredSpirit.movement.currentPos, 0));

        float angle = glm::atan(kindredSpirit.movement.direction.y, kindredSpirit.movement.direction.x);
        angle /= glm::radians(90.0f);
        angle = glm::round(angle);
        angle *= glm::radians(90.0f);
        transform.SetRotation(glm::quat(glm::vec3(glm::radians(90.0f), 0, angle)));
    }
    else
    {
        Engine::Transform &mainTransform = ecsSystem->GetComponent<Engine::Transform>(kindredSpirit.other);

        transform.SetTranslation(mainTransform.GetTranslation() * -1.0f);
        transform.SetRotation(mainTransform.GetRotation() * glm::quat(glm::vec3(0, 0, glm::radians((180.0f)))));
    }
}

void KindredSpiritSystem::HandleDamage(Engine::Entity entity, Engine::Entity other)
{
    EnemyBehaviour& behaviour = ecsSystem->GetComponent<EnemyBehaviour>(entity);
    KindredSpirit& kindredSpirit = ecsSystem->GetComponent<KindredSpirit>(entity);
    KindredSpirit& otherKindredSpirit = ecsSystem->GetComponent<KindredSpirit>(kindredSpirit.other);

    if(!behaviour.isActive) return;

    kindredSpirit.timeWhenLastHit = std::chrono::system_clock::now();
    std::chrono::duration<float> timeDifference = (kindredSpirit.timeWhenLastHit - otherKindredSpirit.timeWhenLastHit);

    if(abs(timeDifference.count()) <= KindredSpirit::maxTimeDifference)
    {
        //Bullet is already destroying itself, so no need to do it here
        //Get the health of the main kindred spirit
        Health &health = kindredSpirit.isMainEntity ? ecsSystem->GetComponent<Health>(entity) : ecsSystem->GetComponent<Health>(kindredSpirit.other);

        health.health--;
        if (health.health <= 0)
        {
            Systems::enemyBehaviourSystem->Kill(entity, EnemyBehaviour::scores[EnemyBehaviour::KindredSpirit]);
            Systems::enemyBehaviourSystem->Kill(kindredSpirit.other, EnemyBehaviour::scores[EnemyBehaviour::KindredSpirit]);
        }
    }
}

void KindredSpiritSystem::Update(float deltaTime)
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

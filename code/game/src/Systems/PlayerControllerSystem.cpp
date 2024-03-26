#include "Engine.h"
#include <iostream>
#include "ECSExtension.h"
#include "Systems/PlayerControllerSystem.h"


static const float SQRT_THREE_FOURTH = 0.866025f;

void PlayerControllerSystem::EntityAdded(Engine::Entity entity)
{
    alivePlayers++;
}

void PlayerControllerSystem::EntityRemoved(Engine::Entity entity)
{
    alivePlayers--;
    if(alivePlayers == 0)
    {
        std::cout << "All players dead" << std::endl;
        Engine::EndGame();
    }
}

void PlayerControllerSystem::Update(float deltaTime)
{
    for (Engine::Entity playerEntity: entities)
    {
        ecsSystem->GetComponent<PlayerController>(playerEntity).stunnedTimer -= deltaTime;
        if(ecsSystem->GetComponent<PlayerController>(playerEntity).isActive)
            HandleInput(playerEntity, deltaTime);
        else
        {
            ecsSystem->GetComponent<PlayerController>(playerEntity).respawnTimer -= deltaTime;
            if(ecsSystem->GetComponent<PlayerController>(playerEntity).respawnTimer <= 0.0f)
                ActivatePlayer(playerEntity);
        }

        ResolveCollisions(playerEntity, deltaTime);
        UpdateUI(playerEntity);
    }
}

void PlayerControllerSystem::ResolveCollisions(Engine::Entity playerEntity, float deltaTime)
{
    Engine::Transform& transform = ecsSystem->GetComponent<Engine::Transform>(playerEntity);
    Engine::BoxCollider& boxCollider = ecsSystem->GetComponent<Engine::BoxCollider>(playerEntity);
    PlayerController& controller = ecsSystem->GetComponent<PlayerController>(playerEntity);

    glm::vec3 averageDirection{0};
    int count = 0;
    for(auto pair : boxCollider.collisions)
    {
        Engine::Entity other = pair.first.other;
        if(ecsSystem->HasComponent<Loot>(other))
        {
            controller.AddScore(ecsSystem->GetComponent<Loot>(other).score);
            RemoveEntityWithChildren(other);
        }
            //If bullet was hit
        else if (ecsSystem->HasComponent<Bullet>(other) && ecsSystem->GetComponent<Bullet>(other).spawner != playerEntity)
        {
            //If bullet is shot from player
            if (ecsSystem->HasComponent<PlayerController>(ecsSystem->GetComponent<Bullet>(other).spawner))
            {
                controller.stunnedTimer = controller.stunnedTime;
            }
            else if(controller.stunnedTimer < 0.0f)
            {
                //Bullet is already destroying itself, so no need to do it here
                Health &health = ecsSystem->GetComponent<Health>(playerEntity);
                health.health--;
                if (health.health <= 0 && controller.respawnTimer <= 0.0f)
                {
                    health.health = health.maxHealth;
                    DeactivatePlayer(playerEntity);
                }
            }
        }
        //If kindredSpirit was hit
        else if (ecsSystem->HasComponent<EnemyBehaviour>(other) && ecsSystem->GetComponent<EnemyBehaviour>(other).behaviour == EnemyBehaviour::KindredSpirit)
        {
            Health &health = ecsSystem->GetComponent<Health>(playerEntity);
            health.health--;
            if (health.health <= 0 && controller.respawnTimer <= 0.0f)
            {
                health.health = health.maxHealth;
                DeactivatePlayer(playerEntity);
            }
        }
        else
        {
            Engine::Transform &otherTransform = ecsSystem->GetComponent<Engine::Transform>(other);
            glm::vec2 distance = transform.GetGlobalTranslation() - otherTransform.GetGlobalTranslation();
            averageDirection += glm::vec3(distance, 0);
            count++;
        }
    }

    if(count != 0)
    {
        averageDirection = RoundToAxis(averageDirection);

        if (glm::length(controller.movementInput) >= inputDeadzone)
            averageDirection *= glm::length(controller.movementInput) * controller.speed;
        transform.AddTranslation(averageDirection * deltaTime);
    }
}

void PlayerControllerSystem::HandleInput(Engine::Entity playerEntity, float deltaTime)
{
    Engine::Transform& transform = ecsSystem->GetComponent<Engine::Transform>(playerEntity);
    PlayerController& controller = ecsSystem->GetComponent<PlayerController>(playerEntity);

    if(controller.stunnedTimer > 0)
    {
        return;
    }

    if(glm::length(controller.movementInput) >= inputDeadzone)
    {
        transform.AddTranslation(glm::vec3(controller.movementInput) * deltaTime * controller.speed);
        controller.lookDirection = glm::normalize(glm::vec3(controller.movementInput));
    }
    if(controller.wasFirePushed)
    {
        controller.wasFirePushed = false;
        if(controller.activeBullets != controller.maxBullets)
        {
            ECSHelper::SpawnBullet(playerEntity, transform.GetGlobalTranslation(),RoundToAxis(controller.lookDirection), controller.bulletSpeed);
            controller.activeBullets++;
        }
    }
    if(controller.wasRevivePushed)
    {
        controller.wasRevivePushed = false;
        Engine::BoxCollider& collider = ecsSystem->GetComponent<Engine::BoxCollider>(playerEntity);
        for(Engine::Entity otherPlayer : entities)
        {
            Engine::BoxCollider& otherCollider = ecsSystem->GetComponent<Engine::BoxCollider>(otherPlayer);

            if(&collider == &otherCollider) continue;
            ecsSystem->GetComponent<Engine::Transform>(otherPlayer).AddTranslation(glm::vec3(0, 0, respawnDistance));
            if(Engine::Systems::collisionSystem->CheckCollision(collider, otherCollider))
            {
                ecsSystem->GetComponent<Engine::Transform>(otherPlayer).AddTranslation(glm::vec3(0, 0, -respawnDistance));
                ActivatePlayer(otherPlayer);
                break;
            }
            ecsSystem->GetComponent<Engine::Transform>(otherPlayer).AddTranslation(glm::vec3(0, 0, -respawnDistance));
        }

    }
}

void PlayerControllerSystem::UpdateUI(Engine::Entity playerEntity)
{
    PlayerController& controller = ecsSystem->GetComponent<PlayerController>(playerEntity);
    if(controller.hasScoreChanged && ecsSystem->HasComponent<Engine::Text>(controller.uiTextScore))
    {
        Engine::Text& uiText = ecsSystem->GetComponent<Engine::Text>(controller.uiTextScore);
        uiText.SetText(std::to_string(controller.score));
        controller.hasScoreChanged = false;

        Engine::Text& uiTextAll = ecsSystem->GetComponent<Engine::Text>(scoreUI);
        uiTextAll.SetText(std::to_string(SumUpAllScores()));
    }
}

glm::vec3 PlayerControllerSystem::RoundToAxis(glm::vec3 vec)
{

    if(vec != glm::vec3(0))
        vec = glm::normalize(vec);

    vec *= SQRT_THREE_FOURTH; //Normalize
    vec.x = roundf(vec.x);
    vec.y = roundf(vec.y);
    vec.z = roundf(vec.z);

    if(vec != glm::vec3(0))
        vec = glm::normalize(vec);
    return vec;
}

void PlayerControllerSystem::DeactivatePlayer(Engine::Entity entity)
{
    CheckIfAllPlayerDead();
    PlayerController& player = ecsSystem->GetComponent<PlayerController>(entity);
    player.isActive = false;
    player.respawnTimer = player.respawnTime;
    ecsSystem->GetComponent<Engine::Transform>(entity).AddTranslation(glm::vec3(0,0, -respawnDistance));
}

void PlayerControllerSystem::ActivatePlayer(Engine::Entity entity)
{
    ecsSystem->GetComponent<PlayerController>(entity).isActive = true;
    ecsSystem->GetComponent<Engine::Transform>(entity).AddTranslation(glm::vec3(0,0, respawnDistance));
}

int PlayerControllerSystem::SumUpAllScores()
{
    int sum = 0;
    for(Engine::Entity player : entities)
    {
        sum += ecsSystem->GetComponent<PlayerController>(player).score;
    }
    return sum;
}

void PlayerControllerSystem::CheckIfAllPlayerDead()
{
    for(Engine::Entity entity : entities)
    {
        PlayerController& controller = ecsSystem->GetComponent<PlayerController>(entity);
        if(controller.isActive)
            break;
    }

    std::cout << "You are so good. You all died. Game over now" << std::endl;
    Engine::EndGame();
}

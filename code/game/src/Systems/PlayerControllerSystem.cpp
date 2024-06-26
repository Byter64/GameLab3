#include "Engine.h"
#include <iostream>
#include "ECSExtension.h"
#include "Systems/PlayerControllerSystem.h"
#include "Scenes/02_Game.h"
#include "Scenes/04_GameLost.h"

extern std::pair<Engine::Entity, Engine::Entity> players;

void PlayerControllerSystem::EntityAdded(Engine::Entity entity)
{

}

void PlayerControllerSystem::EntityRemoved(Engine::Entity entity)
{

}

void PlayerControllerSystem::Update(float deltaTime)
{
    for (Engine::Entity playerEntity: entities)
    {
        PlayerController& playerController = ecsSystem->GetComponent<PlayerController>(playerEntity);
        playerController.stunnedTimer -= deltaTime;
        if(playerController.isActive)
            HandleInput(playerEntity, deltaTime);
        else
        {
            playerController.respawnTimer -= deltaTime;
            if(playerController.respawnTimer <= 0.0f)
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
    for(auto& collision : boxCollider.collisions)
    {
        Engine::Entity other = collision.other;
        if(ecsSystem->HasComponent<Loot>(other))
        {
            controller.AddScore(ecsSystem->GetComponent<Loot>(other).score);
            RemoveEntityWithChildren(other);
        }
        //If bullet was hit
        else if (ecsSystem->HasComponent<Bullet>(other) && ecsSystem->GetComponent<Bullet>(other).spawner != playerEntity)
        {
            //If bullet is shot from player
            if (ecsSystem->HasComponent<PlayerController>(ecsSystem->GetComponent<Bullet>(other).spawner) && controller.stunnedTimer <= 0.0f)
            {
                controller.stunnedTimer = controller.stunnedTime;
            }
            else if(controller.stunnedTimer <= 0.0f)
            {
                glm::vec3 position = transform.GetGlobalTranslation();
                std::cout << ecsSystem->GetComponent<Engine::Name>(playerEntity) << " was hit by " << ecsSystem->GetComponent<Engine::Name>(other) << ". Player position was: (" << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;
                std::string bulletSpawnername = "Unknown";
                if(ecsSystem->HasComponent<Engine::Name>(ecsSystem->GetComponent<Bullet>(other).spawner))
                    bulletSpawnername = ecsSystem->GetComponent<Engine::Name>(ecsSystem->GetComponent<Bullet>(other).spawner);
                std::cout << "Bullet was fired by " << bulletSpawnername << "." << std::endl;
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
        //If kindredSpirit or Assi or Cuball was hit
        else if (ecsSystem->HasComponent<EnemyBehaviour>(other) && (ecsSystem->HasComponent<KindredSpirit>(other) ||
                                                                            ecsSystem->HasComponent<Assi>(other) ||
                                                                            ecsSystem->HasComponent<Cuball>(other)))
        {
            glm::vec3 position = transform.GetGlobalTranslation();
            std::cout << ecsSystem->GetComponent<Engine::Name>(playerEntity) << " was hit by " << ecsSystem->GetComponent<Engine::Name>(other) << ". Player position was: (" << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;
            if(controller.stunnedTimer < 0.0f)
            {
                Health &health = ecsSystem->GetComponent<Health>(playerEntity);
                health.health--;
                if (health.health <= 0 && controller.respawnTimer <= 0.0f)
                {
                    health.health = health.maxHealth;
                    DeactivatePlayer(playerEntity);
                }
            }
        }
        else
        {
            glm::vec2 distance = collision.entityPos - collision.otherPos;
            if(distance != glm::vec2(0))
                distance = glm::normalize(distance);

            averageDirection += glm::vec3(distance, 0);
            count++;
        }
    }

    if(count != 0)
    {
        averageDirection = Miscellaneous::RoundTo8Directions(averageDirection);

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
        glm::vec3 movementInput = controller.movementInput;
        if(glm::length(movementInput) > 1.0f)
            movementInput = glm::normalize(controller.movementInput);
        transform.AddTranslation(movementInput * deltaTime * controller.speed);
        controller.lookDirection = movementInput;
        glm::vec2 rot = Miscellaneous::RoundTo8Directions(movementInput);
        transform.SetRotation(glm::vec3(glm::radians(90.0f),0 , glm::atan(rot.y, rot.x)));
    }
    if(controller.wasFirePushed)
    {
        controller.wasFirePushed = false;
        if(controller.activeBullets != controller.maxBullets)
        {
            sceneManager->GetActiveScene<Game>().CreateBullet(playerEntity, transform.GetGlobalTranslation() + glm::vec3{0, 0, 0.5f},
                                    Miscellaneous::RoundTo8Directions(controller.lookDirection), controller.bulletSpeed);
            controller.activeBullets++;
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

void PlayerControllerSystem::DeactivatePlayer(Engine::Entity entity)
{
    PlayerController& player = ecsSystem->GetComponent<PlayerController>(entity);
    player.isActive = false;
    player.respawnTimer = player.respawnTime;
    ecsSystem->GetComponent<Engine::Transform>(entity).AddTranslation(glm::vec3(0,0, -respawnDistance));
    CheckIfAllPlayerDead();
}

void PlayerControllerSystem::ActivatePlayer(Engine::Entity entity)
{
    ecsSystem->GetComponent<PlayerController>(entity).isActive = true;
    ecsSystem->GetComponent<PlayerController>(entity).respawnTimer = -1.0f;
    ecsSystem->GetComponent<Engine::Transform>(entity).AddTranslation(glm::vec3(0,0, respawnDistance));
    ecsSystem->GetComponent<Health>(entity).health = ecsSystem->GetComponent<Health>(entity).maxHealth;
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
            return;
    }

    std::cout << "You are so good. You all died. Game over now" << std::endl;
    Game::scoreP1 = ecsSystem->GetComponent<PlayerController>(players.first).GetScore();
    if(players.second != Engine::Entity::INVALID_ENTITY_ID)
        Game::scoreP2 = ecsSystem->GetComponent<PlayerController>(players.second).GetScore();
    sceneManager->LoadScene<GameLost>();
}

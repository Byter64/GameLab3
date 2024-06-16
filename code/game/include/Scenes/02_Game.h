#pragma once
#include "ECSHelper.h"

class Game : public Engine::Scene
{
    Engine::Entity wallPrefab = Engine::Entity::INVALID_ENTITY_ID;
    Engine::Entity breakableWallPrefab = Engine::Entity::INVALID_ENTITY_ID;
    Engine::Entity bulletPrefab = Engine::Entity::INVALID_ENTITY_ID;
    Engine::Entity jewelAnimationPrefab = Engine::Entity::INVALID_ENTITY_ID;
    Engine::Entity elevatorPrefab = Engine::Entity::INVALID_ENTITY_ID;
    Engine::Entity hubertusPrefab = Engine::Entity::INVALID_ENTITY_ID;
    Engine::Entity kindredSpiritPrefab = Engine::Entity::INVALID_ENTITY_ID;
    Engine::Entity assiPrefab = Engine::Entity::INVALID_ENTITY_ID;
    Engine::Entity cuballPrefab = Engine::Entity::INVALID_ENTITY_ID;
    Engine::Entity dukePrefab = Engine::Entity::INVALID_ENTITY_ID;

    Engine::Entity pauseText;
    std::shared_ptr<Engine::InputActionButton> pause;
    float pauseStartTime;
    float pauseTextScale = 8;
    static void PauseGame(void*);
public:
    static inline int scoreP1 = -1;
    static inline int scoreP2 = -1;

    void OnStart() override;
    void OnEnd() override;
    void OnUpdate(float deltaTime) override;
    void OnUpdateWithoutPause() override;

    Engine::Entity CreateWall(glm::vec3 position);
    Engine::Entity CreateBreakableWall(glm::vec3 position);
    Engine::Entity CreateBullet(Engine::Entity spawner, glm::vec3 position, glm::vec3 direction, float speed = 4.0f);
    Engine::Entity CreateLoot(glm::vec3 position, int points);
    Engine::Entity CreateElevator(glm::vec3 position, Engine::Entity spawnedEnemy);
    Engine::Entity CreateHubertus(std::pair<int, int> startPos);
    std::pair<Engine::Entity, Engine::Entity> CreateKindredSpirit(std::pair<int, int> startPos);
    Engine::Entity CreateAssi(std::pair<int, int> startPos);
    Engine::Entity CreateCuball(std::pair<int, int> startPos);
    Engine::Entity CreateDuke(std::pair<int, int> startPos);
};

#pragma once
#include "Engine.h"

class ECSHelper
{
public:
    inline static int numberOfEnemyBehaviourPrefabs = 0;

    static void Initialize();
    static Engine::Entity CopyEntity(Engine::Entity entity, bool copyChildren = true);

    static Engine::Entity SpawnWall(glm::vec3 position);
    static Engine::Entity SpawnBreakableWall(glm::vec3 position);
    static Engine::Entity SpawnBullet(Engine::Entity spawner, glm::vec3 position, glm::vec3 direction, float speed = 4.0f);
    static Engine::Entity SpawnLoot(glm::vec3 position, int points);
    static Engine::Entity SpawnElevator(glm::vec3 position, Engine::Entity spawnedEnemy);
    static Engine::Entity CreateHubertus(std::pair<int, int> startPos);
    static std::pair<Engine::Entity, Engine::Entity> CreateKindredSpirit(std::pair<int, int> startPos);
    static Engine::Entity CreateAssi(std::pair<int, int> startPos);
    static Engine::Entity CreateCuball(std::pair<int, int> startPos);
    static Engine::Entity CreateDuke(std::pair<int, int> startPos);
};

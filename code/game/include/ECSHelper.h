#pragma once
#include "Engine.h"

class ECSHelper
{
public:
    static void Initialize();
    static Engine::Entity CopyEntity(Engine::Entity entity, bool copyChildren = true);

    static Engine::Entity SpawnWall(glm::vec3 position);
    static Engine::Entity SpawnBullet(Engine::Entity spawner, glm::vec3 position, glm::vec3 direction, float speed = 4.0f);
    static Engine::Entity SpawnLoot(glm::vec3 position, int points);
    static Engine::Entity SpawnHubertus(std::pair<int, int> startPos);
    static std::pair<Engine::Entity, Engine::Entity> SpawnKindredSpirit(std::pair<int, int> startPos);
};

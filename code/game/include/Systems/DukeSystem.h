#pragma once
#include "Engine.h"
#include "Helpers/Movement.h"
#include <functional>

class DukeSystem : public Engine::System
{
    static inline int attackTime = 11;
    static inline int spawnTime = 29;
    void EntityAdded(Engine::Entity entity);
    void EntityRemoved(Engine::Entity entity);

    void FindNewPosition(Movement &movement, std::function<bool(glm::vec3 startGlobalPos)> condition);
    void FindNewPositionCloseToPlayer(Movement &movement, glm::vec3 playerPos);
    bool FindNewTargetPosition(Movement &movement, std::function<bool(glm::vec3 startGlobalPos)> condition);

    void Update(Engine::Entity entity, float deltaTime);
    void HandleDamage(Engine::Entity entity, Engine::Entity other);

public:
    DukeSystem();

    void Update(float deltaTime);
};

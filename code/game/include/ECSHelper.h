#pragma once
#include "Engine.h"

class ECSHelper
{
public:
    static void Initialize();
    static Engine::Entity CopyEntity(Engine::Entity entity, bool copyChildren = true);

    static Engine::Entity SpawnBullet(Engine::Entity spawner, glm::vec3 position, glm::vec3 direction, float speed = 4.0f);
};

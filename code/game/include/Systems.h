#pragma once
#include "Engine.h"
#include "Systems/BulletSystem.h"
#include "Systems/EnemyBehaviourSystem.h"
#include "Systems/DungeonSystem.h"
#include "Systems/PlayerControllerSystem.h"
#include "Systems/ElevatorSystem.h"
#include "Systems/BreakableWallSystem.h"

namespace Systems
{
    extern std::shared_ptr<BulletSystem> bulletSystem;
    extern std::shared_ptr<EnemyBehaviourSystem> enemyBehaviourSystem;
    extern std::shared_ptr<DungeonSystem> dungeonSystem;
    extern std::shared_ptr<PlayerControllerSystem> playerControllerSystem;
    extern std::shared_ptr<ElevatorSystem> elevatorSystem;
    extern std::shared_ptr<BreakableWallSystem> destroyerSystem;
} // Systems

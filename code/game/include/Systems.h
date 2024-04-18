#pragma once
#include "Engine.h"
#include "Systems/BulletSystem.h"
#include "Systems/EnemyBehaviourSystem.h"
#include "Systems/HubertusSystem.h"
#include "Systems/KindredSpiritSystem.h"
#include "Systems/AssiSystem.h"
#include "Systems/CuballSystem.h"
#include "Systems/DukeSystem.h"
#include "Systems/DungeonSystem.h"
#include "Systems/PlayerControllerSystem.h"
#include "Systems/ElevatorSystem.h"
#include "Systems/BreakableWallSystem.h"

namespace Systems
{
    extern std::shared_ptr<BulletSystem> bulletSystem;
    extern std::shared_ptr<EnemyBehaviourSystem> enemyBehaviourSystem;
    extern std::shared_ptr<HubertusSystem> hubertusSystem;
    extern std::shared_ptr<KindredSpiritSystem> kindredSpiritSystem;
    extern std::shared_ptr<AssiSystem> assiSystem;
    extern std::shared_ptr<CuballSystem> cuballSystem;
    extern std::shared_ptr<DukeSystem> dukeSystem;
    extern std::shared_ptr<DungeonSystem> dungeonSystem;
    extern std::shared_ptr<PlayerControllerSystem> playerControllerSystem;
    extern std::shared_ptr<ElevatorSystem> elevatorSystem;
    extern std::shared_ptr<BreakableWallSystem> destroyerSystem;
} // Systems

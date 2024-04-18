#include "Systems.h"
#include "ECSExtension.h"

namespace Systems
{
    std::shared_ptr<PlayerControllerSystem> playerControllerSystem;
    std::shared_ptr<BulletSystem> bulletSystem;
    std::shared_ptr<DungeonSystem> dungeonSystem;
    std::shared_ptr<EnemyBehaviourSystem> enemyBehaviourSystem;
    std::shared_ptr<HubertusSystem> hubertusSystem;
    std::shared_ptr<KindredSpiritSystem> kindredSpiritSystem;
    std::shared_ptr<AssiSystem> assiSystem;
    std::shared_ptr<CuballSystem> cuballSystem;
    std::shared_ptr<DukeSystem> dukeSystem;
    std::shared_ptr<ElevatorSystem> elevatorSystem;
    std::shared_ptr<BreakableWallSystem> destroyerSystem;
}
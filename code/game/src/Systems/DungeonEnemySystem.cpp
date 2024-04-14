#include "Systems/DungeonEnemySystem.h"
#include "Engine.h"
#include "ECSExtension.h"

extern std::shared_ptr<DungeonSystem> dungeonSystem; //Never change this name, as Systems depend on this symbol

void DungeonEnemySystem::EntityAdded(Engine::Entity entity)
{

}

void DungeonEnemySystem::EntityRemoved(Engine::Entity entity)
{
    dungeonSystem->OnEnemyDestroyed(entity);
}
#include "ECSHelper.h"
#include "Engine.h"
#include "ECSExtension.h"
#include "CollisionLayer.h"

extern std::shared_ptr<Engine::PlayerControllerSystem> playerControllerSystem; //Never change this name, as Systems depend on this symbol
std::shared_ptr<BulletSystem> bulletSystem; //Never change this name, as Systems depend on this symbol
extern std::shared_ptr<Engine::EnemyBehaviourSystem> enemyBehaviourSystem; //Never change this name, as Systems depend on this symbol
extern std::shared_ptr<Engine::DungeonEnemySystem> dungeonEnemySystem; //Never change this name, as Systems depend on this symbol
extern std::shared_ptr<Engine::DungeonSystem> dungeonSystem; //Never change this name, as Systems depend on this symbol

Engine::Entity bulletPrefab = Engine::Entity::INVALID_ENTITY_ID;
Engine::Entity lootPrefab = Engine::Entity::INVALID_ENTITY_ID;

void ECSHelper::Initialize()
{
    ecsSystem->RegisterComponent<Engine::PlayerController>();
    ecsSystem->RegisterComponent<Bullet>();
    ecsSystem->RegisterComponent<Engine::EnemyBehaviour>();
    ecsSystem->RegisterComponent<Engine::Health>();
    ecsSystem->RegisterComponent<Engine::Dungeon>();
    ecsSystem->RegisterComponent<Loot>();
    //When adding new components here, don't forget to add them to EntityUtilities::CopyEntity, too!!!!!

    playerControllerSystem = ecsSystem->RegisterSystem<Engine::PlayerControllerSystem>();
    Engine::Signature  playerControllerSignature;
    playerControllerSignature.set(ecsSystem->GetComponentType<Engine::Transform>());
    playerControllerSignature.set(ecsSystem->GetComponentType<Engine::PlayerController>());
    playerControllerSignature.set(ecsSystem->GetComponentType<Engine::BoxCollider>());
    playerControllerSignature.set(ecsSystem->GetComponentType<Engine::Health>());
    ecsSystem->SetSystemSignature<Engine::PlayerControllerSystem>(playerControllerSignature);

    bulletSystem = ecsSystem->RegisterSystem<BulletSystem>();
    Engine::Signature bulletSignature;
    bulletSignature.set(ecsSystem->GetComponentType<Engine::Transform>());
    bulletSignature.set(ecsSystem->GetComponentType<Bullet>());
    bulletSignature.set(ecsSystem->GetComponentType<Engine::BoxCollider>());
    ecsSystem->SetSystemSignature<BulletSystem>(bulletSignature);

    enemyBehaviourSystem = ecsSystem->RegisterSystem<Engine::EnemyBehaviourSystem>();
    Engine::Signature enemyBehaviourSignature;
    enemyBehaviourSignature.set(ecsSystem->GetComponentType<Engine::Transform>());
    enemyBehaviourSignature.set(ecsSystem->GetComponentType<Engine::EnemyBehaviour>());
    enemyBehaviourSignature.set(ecsSystem->GetComponentType<Engine::BoxCollider>());
    enemyBehaviourSignature.set(ecsSystem->GetComponentType<Engine::Health>());
    ecsSystem->SetSystemSignature<Engine::EnemyBehaviourSystem>(enemyBehaviourSignature);

    dungeonEnemySystem = ecsSystem->RegisterSystem<Engine::DungeonEnemySystem>();
    Engine::Signature dungeonEnemySignature;
    dungeonEnemySignature.set(ecsSystem->GetComponentType<Engine::EnemyBehaviour>());
    ecsSystem->SetSystemSignature<Engine::DungeonEnemySystem>(dungeonEnemySignature);

    dungeonSystem = ecsSystem->RegisterSystem<Engine::DungeonSystem>();
    Engine::Signature dungeonSignature;
    dungeonSignature.set(ecsSystem->GetComponentType<Engine::Dungeon>());
    ecsSystem->SetSystemSignature<Engine::DungeonSystem>(dungeonSignature);
}

Engine::Entity ECSHelper::CopyEntity(Engine::Entity entity, bool copyChildren)
{
     Engine::Entity newEntity = Engine::CopyEntity(entity, copyChildren);

    if(ecsSystem->HasComponent<Bullet>(entity))
        ecsSystem->AddComponent(newEntity, ecsSystem->GetComponent<Bullet>(entity));
    if(ecsSystem->HasComponent<Loot>(entity))
        ecsSystem->AddComponent(newEntity, ecsSystem->GetComponent<Loot>(entity));

    return newEntity;
}

Engine::Entity ECSHelper::SpawnBullet(Engine::Entity spawner, glm::vec3 position, glm::vec3 direction, float speed)
{
    if(bulletPrefab == Engine::Entity::INVALID_ENTITY_ID)
    {
        bulletPrefab = Engine::ImportGLTF(Engine::Files::ASSETS / "Graphics\\Models\\Bullet\\Bullet.glb")[0];
        ecsSystem->GetComponent<Engine::Transform>(bulletPrefab).SetScale(glm::vec3(0.0f));
    }

    Engine::Entity entity = CopyEntity(bulletPrefab);
    ecsSystem->GetComponent<Engine::Transform>(entity).SetScale(glm::vec3(0.2f));
    ecsSystem->GetComponent<Engine::Transform>(entity).SetTranslation(position);
    ecsSystem->GetComponent<Engine::Transform>(entity).SetRotation(glm::quat(glm::vec3(glm::radians(90.0f), 0, glm::atan(direction.y, direction.x) + glm::radians(90.0f))));

    Bullet& bullet = ecsSystem->AddComponent<Bullet>(entity);
    bullet.velocity = glm::normalize(direction) * speed;
    bullet.spawner = spawner;

    Engine::BoxCollider& collider = ecsSystem->AddComponent<Engine::BoxCollider>(entity);
    collider.size = glm::vec3(1, 1, 1);
    collider.isStatic = false;
    collider.layer = static_cast<unsigned char>(CollisionLayer::Bullet);
    collider.collisions.clear();

    return entity;
}

Engine::Entity ECSHelper::SpawnLoot(glm::vec3 position, int points)
{
    if(lootPrefab == Engine::Entity::INVALID_ENTITY_ID)
    {
        lootPrefab = Engine::ImportGLTF(Engine::Files::ASSETS / "Graphics\\Models\\Jewel\\Jewel.glb")[0];
        ecsSystem->GetComponent<Engine::Transform>(lootPrefab).SetScale(glm::vec3(0));
        ecsSystem->AddComponent<Loot>(lootPrefab, {0});
    }

    Engine::Entity loot = CopyEntity(lootPrefab);

    ecsSystem->GetComponent<Engine::Transform>(loot).SetScale(glm::vec3(1));
    ecsSystem->GetComponent<Engine::Transform>(loot).SetTranslation(position);
    ecsSystem->GetComponent<Loot>(loot).score = points;

    Engine::BoxCollider& collider = ecsSystem->AddComponent<Engine::BoxCollider>(loot);
    collider.size = glm::vec3(1,1, 1000);
    collider.position = glm::vec3 (0);
    collider.isStatic = true;
    collider.layer = static_cast<unsigned char>(CollisionLayer::Collectible);

    return loot;
}
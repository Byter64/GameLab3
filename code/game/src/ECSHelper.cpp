#include "ECSHelper.h"
#include "Engine.h"
#include "ECSExtension.h"
#include "CollisionLayer.h"
#include "GameDefines.h"

extern std::shared_ptr<PlayerControllerSystem> playerControllerSystem; //Never change this name, as Systems depend on this symbol
std::shared_ptr<BulletSystem> bulletSystem; //Never change this name, as Systems depend on this symbol
extern std::shared_ptr<DungeonEnemySystem> dungeonEnemySystem; //Never change this name, as Systems depend on this symbol
extern std::shared_ptr<DungeonSystem> dungeonSystem; //Never change this name, as Systems depend on this symbol
std::shared_ptr<EnemyBehaviourSystem> enemyBehaviourSystem; //Never change this name, as Systems depend on this symbol

Engine::Entity wallPrefab = Engine::Entity::INVALID_ENTITY_ID;
Engine::Entity bulletPrefab = Engine::Entity::INVALID_ENTITY_ID;
Engine::Entity lootPrefab = Engine::Entity::INVALID_ENTITY_ID;
Engine::Entity hubertusPrefab = Engine::Entity::INVALID_ENTITY_ID;
Engine::Entity kindredSpiritPrefabMain = Engine::Entity::INVALID_ENTITY_ID;
Engine::Entity kindredSpiritPrefabSecondary = Engine::Entity::INVALID_ENTITY_ID;

void ECSHelper::Initialize()
{
    ecsSystem->RegisterComponent<PlayerController>();
    ecsSystem->RegisterComponent<Bullet>();
    ecsSystem->RegisterComponent<EnemyBehaviour>();
    ecsSystem->RegisterComponent<Health>();
    ecsSystem->RegisterComponent<Dungeon>();
    ecsSystem->RegisterComponent<Loot>();
    //When adding new components here, don't forget to add them to EntityUtilities::CopyEntity, too!!!!!

    playerControllerSystem = ecsSystem->RegisterSystem<PlayerControllerSystem>();
    Engine::Signature  playerControllerSignature;
    playerControllerSignature.set(ecsSystem->GetComponentType<Engine::Transform>());
    playerControllerSignature.set(ecsSystem->GetComponentType<PlayerController>());
    playerControllerSignature.set(ecsSystem->GetComponentType<Engine::BoxCollider>());
    playerControllerSignature.set(ecsSystem->GetComponentType<Health>());
    ecsSystem->SetSystemSignature<PlayerControllerSystem>(playerControllerSignature);

    bulletSystem = ecsSystem->RegisterSystem<BulletSystem>();
    Engine::Signature bulletSignature;
    bulletSignature.set(ecsSystem->GetComponentType<Engine::Transform>());
    bulletSignature.set(ecsSystem->GetComponentType<Bullet>());
    bulletSignature.set(ecsSystem->GetComponentType<Engine::BoxCollider>());
    ecsSystem->SetSystemSignature<BulletSystem>(bulletSignature);

    enemyBehaviourSystem = ecsSystem->RegisterSystem<EnemyBehaviourSystem>();
    Engine::Signature enemyBehaviourSignature;
    enemyBehaviourSignature.set(ecsSystem->GetComponentType<Engine::Transform>());
    enemyBehaviourSignature.set(ecsSystem->GetComponentType<EnemyBehaviour>());
    enemyBehaviourSignature.set(ecsSystem->GetComponentType<Engine::BoxCollider>());
    enemyBehaviourSignature.set(ecsSystem->GetComponentType<Health>());
    ecsSystem->SetSystemSignature<EnemyBehaviourSystem>(enemyBehaviourSignature);

    dungeonEnemySystem = ecsSystem->RegisterSystem<DungeonEnemySystem>();
    Engine::Signature dungeonEnemySignature;
    dungeonEnemySignature.set(ecsSystem->GetComponentType<EnemyBehaviour>());
    ecsSystem->SetSystemSignature<DungeonEnemySystem>(dungeonEnemySignature);

    dungeonSystem = ecsSystem->RegisterSystem<DungeonSystem>();
    Engine::Signature dungeonSignature;
    dungeonSignature.set(ecsSystem->GetComponentType<Dungeon>());
    ecsSystem->SetSystemSignature<DungeonSystem>(dungeonSignature);
}

Engine::Entity ECSHelper::CopyEntity(Engine::Entity entity, bool copyChildren)
{
     Engine::Entity newEntity = Engine::CopyEntity(entity, copyChildren);

    if(ecsSystem->HasComponent<Bullet>(entity))
        ecsSystem->AddComponent(newEntity, ecsSystem->GetComponent<Bullet>(entity));
    if(ecsSystem->HasComponent<Loot>(entity))
        ecsSystem->AddComponent(newEntity, ecsSystem->GetComponent<Loot>(entity));
    if(ecsSystem->HasComponent<Health>(entity))
        ecsSystem->AddComponent(newEntity, ecsSystem->GetComponent<Health>(entity));
    if(ecsSystem->HasComponent<Dungeon>(entity))
        ecsSystem->AddComponent(newEntity, ecsSystem->GetComponent<Dungeon>(entity));
    if(ecsSystem->HasComponent<PlayerController>(entity))
        ecsSystem->AddComponent(newEntity, ecsSystem->GetComponent<PlayerController>(entity));

    return newEntity;
}

Engine::Entity ECSHelper::SpawnWall(glm::vec3 position)
{
    if(wallPrefab == Engine::Entity::INVALID_ENTITY_ID)
    {
        wallPrefab = Engine::ImportGLTF(Engine::Files::ASSETS / "Graphics\\Models\\Wall\\Wall.glb")[0];
        ecsSystem->GetComponent<Engine::Transform>(wallPrefab).SetScale(glm::vec3(0.0f));
    }

    Engine::Entity wall = CopyEntity(wallPrefab);

    ecsSystem->GetComponent<Engine::Transform>(wall).SetTranslation(position);
    ecsSystem->GetComponent<Engine::Transform>(wall).SetScale(glm::vec3(1.0f));

    Engine::BoxCollider& wallCollider = ecsSystem->AddComponent<Engine::BoxCollider>(wall);
    wallCollider.size = glm::vec3(1,1, 1000);
    wallCollider.position = glm::vec3 (0);
    wallCollider.isStatic = true;
    wallCollider.layer = static_cast<unsigned char>(CollisionLayer::Dungeon);

    return wall;
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
        lootPrefab = Engine::ImportGLTF(Engine::Files::ASSETS / "Graphics\\Models\\Jewel\\Jewel.glb", "Loot_")[0];
        ecsSystem->GetComponent<Engine::Transform>(lootPrefab).SetScale(glm::vec3(0));
        ecsSystem->GetComponent<Engine::Transform>(lootPrefab).SetRotation(glm::quat(glm::vec3(glm::radians(22.5f),0,0)));
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

    Engine::Systems::animationSystem->PlayAnimation(loot, "Loot_Hovering", true,0, 1);

    return loot;
}

Engine::Entity ECSHelper::SpawnHubertus(std::pair<int, int> startPos)
{
    if(hubertusPrefab == Engine::Entity::INVALID_ENTITY_ID)
    {
        hubertusPrefab = Engine::ImportGLTF(Engine::Files::ASSETS/ "Graphics\\Models\\Hubertus\\Hubertus.glb")[0];
        ecsSystem->GetComponent<Engine::Transform>(hubertusPrefab).SetRotation(glm::quat(glm::vec3(glm::radians(90.0f),0,0)));
        ecsSystem->GetComponent<Engine::Transform>(hubertusPrefab).SetScale(glm::vec3(0.0f));
    }
    Engine::Entity enemy = CopyEntity(hubertusPrefab, true);
    ecsSystem->GetComponent<Engine::Transform>(enemy).SetScale(glm::vec3(1.0f));
    ecsSystem->AddComponent<Engine::BoxCollider>(enemy, Engine::BoxCollider());
    ecsSystem->GetComponent<Engine::BoxCollider>(enemy).size = glm::vec3(0.5f);
    ecsSystem->GetComponent<Engine::BoxCollider>(enemy).layer = static_cast<unsigned char>(CollisionLayer::Enemy);
    EnemyBehaviour& behaviour = ecsSystem->AddComponent<EnemyBehaviour>(enemy);
    behaviour.behaviour = EnemyBehaviour::Hubertus;
    behaviour.startPos = startPos;
    behaviour.speed = Defines::Float("Hubertus_Speed");
    behaviour.bulletSpeed = Defines::Float("Hubertus_BulletSpeed");
    ecsSystem->AddComponent<Health>(enemy, Health{Defines::Int("Hubertus_Health")});

    return enemy;
}

//This function will spawn one at the given position and one at startPos * -1
std::pair<Engine::Entity, Engine::Entity> ECSHelper::SpawnKindredSpirit(std::pair<int, int> startPos)
{
    if(kindredSpiritPrefabMain == Engine::Entity::INVALID_ENTITY_ID)
    {
        kindredSpiritPrefabMain = Engine::ImportGLTF(Engine::Files::ASSETS / "Graphics\\Models\\Debug\\Debug.glb")[0];
        ecsSystem->GetComponent<Engine::Transform>(kindredSpiritPrefabMain).SetRotation(glm::quat(glm::vec3(glm::radians(90.0f), 0, 0)));
        ecsSystem->GetComponent<Engine::Transform>(kindredSpiritPrefabMain).SetScale(glm::vec3(0.0f));

        kindredSpiritPrefabSecondary = Engine::ImportGLTF(Engine::Files::ASSETS / "Graphics\\Models\\Debug\\Debug.glb")[0];
        ecsSystem->GetComponent<Engine::MeshRenderer>(kindredSpiritPrefabSecondary).primitiveData.front().material.baseColorFactor = {0.0f,1.0f,1.0f,1.0f};
        ecsSystem->GetComponent<Engine::Transform>(kindredSpiritPrefabSecondary).SetRotation(glm::quat(glm::vec3(glm::radians(90.0f), 0, 0)));
        ecsSystem->GetComponent<Engine::Transform>(kindredSpiritPrefabSecondary).SetScale(glm::vec3(0.0f));
    }

    Engine::Entity enemy1 = CopyEntity(kindredSpiritPrefabMain, true);
    ecsSystem->GetComponent<Engine::Transform>(enemy1).SetScale(glm::vec3(1.0f));

    ecsSystem->AddComponent<Engine::BoxCollider>(enemy1, Engine::BoxCollider());
    Engine::BoxCollider& coll1 = ecsSystem->GetComponent<Engine::BoxCollider>(enemy1);
    coll1.size = glm::vec3(0.5f);
    coll1.layer = static_cast<unsigned char>(CollisionLayer::Enemy);

    ecsSystem->AddComponent<EnemyBehaviour>(enemy1, EnemyBehaviour());
    EnemyBehaviour& behav1 = ecsSystem->GetComponent<EnemyBehaviour>(enemy1);
    behav1.behaviour = EnemyBehaviour::KindredSpirit;
    behav1.startPos = startPos;
    behav1.enemyExtra.kindredSpirit.isMainEntity = true;
    behav1.speed = 3.0f;

    ecsSystem->AddComponent<Health>(enemy1, Health());
    ecsSystem->GetComponent<Health>(enemy1).health = 1;

    Engine::Entity enemy2 = CopyEntity(kindredSpiritPrefabSecondary, true);
    ecsSystem->GetComponent<Engine::Transform>(enemy2).SetScale(glm::vec3(1.0f));

    ecsSystem->AddComponent<Engine::BoxCollider>(enemy2, Engine::BoxCollider());
    Engine::BoxCollider& coll2 = ecsSystem->GetComponent<Engine::BoxCollider>(enemy2);
    coll2.size = glm::vec3(0.5f);
    coll2.layer = static_cast<unsigned char>(CollisionLayer::Enemy);

    ecsSystem->AddComponent<EnemyBehaviour>(enemy2, EnemyBehaviour());
    EnemyBehaviour& behav2 = ecsSystem->GetComponent<EnemyBehaviour>(enemy2);
    behav2.behaviour = EnemyBehaviour::KindredSpirit;
    behav2.startPos = {startPos.first * -1, startPos.second * -1};
    behav2.enemyExtra.kindredSpirit.isMainEntity = false;
    behav2.enemyExtra.kindredSpirit.other = enemy1;
    ecsSystem->AddComponent<Health>(enemy2, Health());
    ecsSystem->GetComponent<Health>(enemy2).health = 1;

    behav1.enemyExtra.kindredSpirit.other = enemy2;

    return {enemy1, enemy2};
}

#include "ECSHelper.h"
#include "Engine.h"
#include "ECSExtension.h"
#include "CollisionLayer.h"
#include "GameDefines.h"

std::shared_ptr<PlayerControllerSystem> playerControllerSystem; //Never change this name, as Systems depend on this symbol
std::shared_ptr<BulletSystem> bulletSystem; //Never change this name, as Systems depend on this symbol
std::shared_ptr<DungeonEnemySystem> dungeonEnemySystem; //Never change this name, as Systems depend on this symbol
std::shared_ptr<DungeonSystem> dungeonSystem; //Never change this name, as Systems depend on this symbol
std::shared_ptr<EnemyBehaviourSystem> enemyBehaviourSystem; //Never change this name, as Systems depend on this symbol
std::shared_ptr<ElevatorSystem> elevatorSystem;
std::shared_ptr<BreakableWallSystem> destroyerSystem;

Engine::Entity wallPrefab = Engine::Entity::INVALID_ENTITY_ID;
Engine::Entity breakableWallPrefab = Engine::Entity::INVALID_ENTITY_ID;
Engine::Entity bulletPrefab = Engine::Entity::INVALID_ENTITY_ID;
Engine::Entity lootPrefab = Engine::Entity::INVALID_ENTITY_ID;
Engine::Entity elevatorPrefab = Engine::Entity::INVALID_ENTITY_ID;
Engine::Entity hubertusPrefab = Engine::Entity::INVALID_ENTITY_ID;
Engine::Entity kindredSpiritPrefab = Engine::Entity::INVALID_ENTITY_ID;
Engine::Entity assiPrefab = Engine::Entity::INVALID_ENTITY_ID;
Engine::Entity cuballPrefab = Engine::Entity::INVALID_ENTITY_ID;
Engine::Entity dukePrefab = Engine::Entity::INVALID_ENTITY_ID;

void ECSHelper::Initialize()
{
    ecsSystem->RegisterComponent<PlayerController>();
    ecsSystem->RegisterComponent<Bullet>();
    ecsSystem->RegisterComponent<EnemyBehaviour>();
    ecsSystem->RegisterComponent<Health>();
    ecsSystem->RegisterComponent<Dungeon>();
    ecsSystem->RegisterComponent<Loot>();
    ecsSystem->RegisterComponent<Elevator>();
    ecsSystem->RegisterComponent<BreakableWall>();
    //When adding new components here, don't forget to add them to ECSHelper::CopyEntity, too!!!!!

    playerControllerSystem = ecsSystem->RegisterSystem<PlayerControllerSystem>();
    Engine::Signature  playerControllerSignature;
    playerControllerSignature.set(ecsSystem->GetComponentType<Engine::Transform>());
    playerControllerSignature.set(ecsSystem->GetComponentType<PlayerController>());
    playerControllerSignature.set(ecsSystem->GetComponentType<Engine::BoxCollider>());
    playerControllerSignature.set(ecsSystem->GetComponentType<Health>());
    ecsSystem->AddSystemSignature<PlayerControllerSystem>(playerControllerSignature);

    bulletSystem = ecsSystem->RegisterSystem<BulletSystem>();
    Engine::Signature bulletSignature;
    bulletSignature.set(ecsSystem->GetComponentType<Engine::Transform>());
    bulletSignature.set(ecsSystem->GetComponentType<Bullet>());
    bulletSignature.set(ecsSystem->GetComponentType<Engine::BoxCollider>());
    ecsSystem->AddSystemSignature<BulletSystem>(bulletSignature);

    enemyBehaviourSystem = ecsSystem->RegisterSystem<EnemyBehaviourSystem>();
    Engine::Signature enemyBehaviourSignature;
    enemyBehaviourSignature.set(ecsSystem->GetComponentType<Engine::Transform>());
    enemyBehaviourSignature.set(ecsSystem->GetComponentType<EnemyBehaviour>());
    enemyBehaviourSignature.set(ecsSystem->GetComponentType<Engine::BoxCollider>());
    enemyBehaviourSignature.set(ecsSystem->GetComponentType<Health>());
    ecsSystem->AddSystemSignature<EnemyBehaviourSystem>(enemyBehaviourSignature);

    dungeonEnemySystem = ecsSystem->RegisterSystem<DungeonEnemySystem>();
    Engine::Signature dungeonEnemySignature;
    dungeonEnemySignature.set(ecsSystem->GetComponentType<EnemyBehaviour>());
    ecsSystem->AddSystemSignature<DungeonEnemySystem>(dungeonEnemySignature);

    dungeonSystem = ecsSystem->RegisterSystem<DungeonSystem>();
    Engine::Signature dungeonSignature;
    dungeonSignature.set(ecsSystem->GetComponentType<Dungeon>());
    ecsSystem->AddSystemSignature<DungeonSystem>(dungeonSignature);

    elevatorSystem = ecsSystem->RegisterSystem<ElevatorSystem>();
    Engine::Signature elevatorSignature;
    elevatorSignature.set(ecsSystem->GetComponentType<Elevator>());
    ecsSystem->AddSystemSignature<ElevatorSystem>(elevatorSignature);

    destroyerSystem = ecsSystem->RegisterSystem<BreakableWallSystem>();
    Engine::Signature destroyerSignature;
    destroyerSignature.set(ecsSystem->GetComponentType<Health>());
    destroyerSignature.set(ecsSystem->GetComponentType<BreakableWall>());
    destroyerSignature.set(ecsSystem->GetComponentType<Engine::BoxCollider>());
    ecsSystem->AddSystemSignature<BreakableWallSystem>(destroyerSignature);
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
    if(ecsSystem->HasComponent<Elevator>(entity))
        ecsSystem->AddComponent(newEntity, ecsSystem->GetComponent<Elevator>(entity));
    if(ecsSystem->HasComponent<Dungeon>(entity))
        ecsSystem->AddComponent(newEntity, ecsSystem->GetComponent<Dungeon>(entity));

    return newEntity;
}

Engine::Entity ECSHelper::SpawnWall(glm::vec3 position)
{
    if(wallPrefab == Engine::Entity::INVALID_ENTITY_ID)
    {
        wallPrefab = Engine::ImportGLTF(Engine::Files::ASSETS / "Graphics\\Models\\Wall.glb")[0];
        ecsSystem->GetComponent<Engine::Transform>(wallPrefab).SetScale(glm::vec3(0.0f));
    }

    Engine::Entity wall = CopyEntity(wallPrefab);

    ecsSystem->GetComponent<Engine::Transform>(wall).SetTranslation(position);
    ecsSystem->GetComponent<Engine::Transform>(wall).SetScale(glm::vec3(1.0f));

    return wall;
}

Engine::Entity ECSHelper::SpawnBreakableWall(glm::vec3 position)
{
    if(breakableWallPrefab == Engine::Entity::INVALID_ENTITY_ID)
    {
        breakableWallPrefab = Engine::ImportGLTF(Engine::Files::ASSETS / "Graphics\\Models\\breakableWall.glb")[0];
        ecsSystem->AddComponent<Health>(breakableWallPrefab, {10, 10});
        ecsSystem->GetComponent<Engine::Transform>(breakableWallPrefab).SetScale(glm::vec3(0.0f));
    }

    Engine::Entity wall = CopyEntity(breakableWallPrefab);

    ecsSystem->GetComponent<Engine::Transform>(wall).SetTranslation(position);
    ecsSystem->GetComponent<Engine::Transform>(wall).SetScale(glm::vec3(1.0f));
    ecsSystem->AddComponent<BreakableWall>(wall);

    Engine::BoxCollider& collider = ecsSystem->AddComponent<Engine::BoxCollider>(wall);
    collider.isStatic = true;
    collider.size = glm::vec3(1);
    collider.layer = (int)CollisionLayer::Dungeon;

    return wall;
}

Engine::Entity ECSHelper::SpawnBullet(Engine::Entity spawner, glm::vec3 position, glm::vec3 direction, float speed)
{
    if(bulletPrefab == Engine::Entity::INVALID_ENTITY_ID)
    {
        bulletPrefab = Engine::ImportGLTF(Engine::Files::ASSETS / "Graphics\\Models\\Bullet.glb")[0];
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
        lootPrefab = Engine::ImportGLTF(Engine::Files::ASSETS / "Graphics\\Models\\Jewel.glb", "Loot_")[0];
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


Engine::Entity ECSHelper::SpawnElevator(glm::vec3 position, Engine::Entity spawnedEnemy)
{
    if(elevatorPrefab == Engine::Entity::INVALID_ENTITY_ID)
    {
        elevatorPrefab = Engine::ImportGLTF(Engine::Files::ASSETS/ "Graphics\\Models\\Elevator.glb", "Elevator_")[0];
        ecsSystem->GetComponent<Engine::Transform>(elevatorPrefab).SetRotation(glm::quat(glm::vec3(glm::radians(90.0f),0,glm::radians(-90.0f))));
        ecsSystem->GetComponent<Engine::Transform>(elevatorPrefab).SetScale(glm::vec3(0.0f));
    }
    Engine::Entity elevator = CopyEntity(elevatorPrefab, true);
    ecsSystem->GetComponent<Engine::Transform>(elevator).SetScale(glm::vec3(1.0f));
    ecsSystem->GetComponent<Engine::Transform>(elevator).SetTranslation(position);
    ecsSystem->AddComponent<Elevator>(elevator);
    ecsSystem->GetComponent<Elevator>(elevator).spawnedEnemy = spawnedEnemy;
    return elevator;
}

Engine::Entity ECSHelper::SpawnHubertus(std::pair<int, int> startPos)
{
    if(hubertusPrefab == Engine::Entity::INVALID_ENTITY_ID)
    {
        hubertusPrefab = Engine::ImportGLTF(Engine::Files::ASSETS/ "Graphics\\Models\\Hubertus.glb")[0];
        ecsSystem->GetComponent<Engine::Transform>(hubertusPrefab).SetRotation(glm::quat(glm::vec3(glm::radians(90.0f),0,0)));
        ecsSystem->GetComponent<Engine::Transform>(hubertusPrefab).SetScale(glm::vec3(0.0f));
    }
    Engine::Entity enemy = CopyEntity(hubertusPrefab, true);
    ecsSystem->GetComponent<Engine::Transform>(enemy).SetScale(glm::vec3(1.0f));
    ecsSystem->AddComponent<Engine::BoxCollider>(enemy, Engine::BoxCollider());
    ecsSystem->GetComponent<Engine::BoxCollider>(enemy).size = glm::vec3(0.5f);
    ecsSystem->GetComponent<Engine::BoxCollider>(enemy).layer = static_cast<unsigned char>(CollisionLayer::Enemy);
    EnemyBehaviour behaviour;
    behaviour.behaviour = EnemyBehaviour::Hubertus;
    behaviour.startPos = startPos;
    behaviour.speed = Defines::Float("Hubertus_Speed");
    behaviour.bulletSpeed = Defines::Float("Hubertus_BulletSpeed");
    behaviour.spawnTime = Engine::Systems::timeManager->GetTimeSinceStartup();
    ecsSystem->AddComponent<EnemyBehaviour>(enemy, behaviour);
    ecsSystem->AddComponent<Health>(enemy, Health{Defines::Int("Hubertus_Health"), Defines::Int("Hubertus_Health")});

    return enemy;
}

//This function will spawn one at the given position and one at startPos * -1
std::pair<Engine::Entity, Engine::Entity> ECSHelper::SpawnKindredSpirit(std::pair<int, int> startPos)
{
    if(kindredSpiritPrefab == Engine::Entity::INVALID_ENTITY_ID)
    {
        kindredSpiritPrefab = Engine::ImportGLTF(Engine::Files::ASSETS / "Graphics\\Models\\Debug.glb")[0];
        ecsSystem->GetComponent<Engine::Transform>(kindredSpiritPrefab).SetRotation(glm::quat(glm::vec3(glm::radians(90.0f), 0, 0)));
        ecsSystem->GetComponent<Engine::Transform>(kindredSpiritPrefab).SetScale(glm::vec3(0.0f));
    }

    Engine::Entity enemy1 = CopyEntity(kindredSpiritPrefab, true);
    ecsSystem->GetComponent<Engine::Transform>(enemy1).SetScale(glm::vec3(1.0f));

    ecsSystem->AddComponent<Engine::BoxCollider>(enemy1, Engine::BoxCollider());
    Engine::BoxCollider& coll1 = ecsSystem->GetComponent<Engine::BoxCollider>(enemy1);
    coll1.size = glm::vec3(0.5f);
    coll1.layer = static_cast<unsigned char>(CollisionLayer::Enemy);

    EnemyBehaviour behav1;
    behav1.enemyExtra.kindredSpirit = KindredSpiritExtra();
    behav1.behaviour = EnemyBehaviour::KindredSpirit;
    behav1.startPos = startPos;
    behav1.enemyExtra.kindredSpirit.isMainEntity = true;
    behav1.speed = Defines::Float("KindredSpirit_Speed");
    behav1.spawnTime = Engine::Systems::timeManager->GetTimeSinceStartup();
    ecsSystem->AddComponent<Health>(enemy1, Health{Defines::Int("KindredSpirit_Health"), Defines::Int("KindredSpirit_Health")});

    Engine::Entity enemy2 = CopyEntity(kindredSpiritPrefab, true);
    ecsSystem->GetComponent<Engine::Transform>(enemy2).SetScale(glm::vec3(1.0f));

    ecsSystem->AddComponent<Engine::BoxCollider>(enemy2, Engine::BoxCollider());
    Engine::BoxCollider& coll2 = ecsSystem->GetComponent<Engine::BoxCollider>(enemy2);
    coll2.size = glm::vec3(0.5f);
    coll2.layer = static_cast<unsigned char>(CollisionLayer::Enemy);

    EnemyBehaviour behav2;
    behav2.enemyExtra.kindredSpirit = KindredSpiritExtra();
    behav2.behaviour = EnemyBehaviour::KindredSpirit;
    behav2.startPos = startPos; //this value needs to be the same as for the first spirit
    behav2.speed = Defines::Float("KindredSpirit_Speed");
    behav2.enemyExtra.kindredSpirit.isMainEntity = false;
    behav2.enemyExtra.kindredSpirit.other = enemy1;
    behav2.spawnTime = Engine::Systems::timeManager->GetTimeSinceStartup();
    ecsSystem->AddComponent<Health>(enemy2, Health{Defines::Int("KindredSpirit_Health"), Defines::Int("KindredSpirit_Health")});

    behav1.enemyExtra.kindredSpirit.other = enemy2;
    ecsSystem->AddComponent<EnemyBehaviour>(enemy1, behav1);
    ecsSystem->AddComponent<EnemyBehaviour>(enemy2, behav2);


    glm::vec4 colour = KindredSpiritExtra::colours.front();
    KindredSpiritExtra::colours.pop();
    for(Engine::MeshRenderer* renderer : Engine::GetComponentsInChildren<Engine::MeshRenderer>(enemy1))
        for(Engine::MeshRenderer::PrimitiveData &primitive : renderer->primitiveData)
            primitive.material.baseColorFactor = colour;
    for(Engine::MeshRenderer* renderer : Engine::GetComponentsInChildren<Engine::MeshRenderer>(enemy2))
        for(Engine::MeshRenderer::PrimitiveData &primitive : renderer->primitiveData)
            primitive.material.baseColorFactor = colour;

    return {enemy1, enemy2};
}


Engine::Entity ECSHelper::SpawnAssi(std::pair<int, int> startPos)
{
    if(assiPrefab == Engine::Entity::INVALID_ENTITY_ID)
    {
        assiPrefab = Engine::ImportGLTF(Engine::Files::ASSETS/ "Graphics\\Models\\Assi.glb")[0];
        ecsSystem->GetComponent<Engine::Transform>(assiPrefab).SetRotation(glm::quat(glm::vec3(glm::radians(90.0f),0,0)));
        ecsSystem->GetComponent<Engine::Transform>(assiPrefab).SetScale(glm::vec3(0.0f));
    }
    Engine::Entity enemy = CopyEntity(assiPrefab, true);
    ecsSystem->GetComponent<Engine::Transform>(enemy).SetScale(glm::vec3(1.0f));
    ecsSystem->AddComponent<Engine::BoxCollider>(enemy, Engine::BoxCollider());
    ecsSystem->GetComponent<Engine::BoxCollider>(enemy).size = glm::vec3(0.5f);
    ecsSystem->GetComponent<Engine::BoxCollider>(enemy).layer = static_cast<unsigned char>(CollisionLayer::Enemy);
    EnemyBehaviour behaviour;
    behaviour.enemyExtra.assi = AssiExtra();
    behaviour.behaviour = EnemyBehaviour::Assi;
    behaviour.startPos = startPos;
    behaviour.speed = Defines::Float("Assi_Speed");
    behaviour.spawnTime = Engine::Systems::timeManager->GetTimeSinceStartup();
    ecsSystem->AddComponent<EnemyBehaviour>(enemy, behaviour);
    ecsSystem->AddComponent<Health>(enemy, Health{Defines::Int("Assi_Health"), Defines::Int("Assi_Health")});

    return enemy;
}


Engine::Entity ECSHelper::SpawnCuball(std::pair<int, int> startPos)
{
    if(cuballPrefab == Engine::Entity::INVALID_ENTITY_ID)
    {
        cuballPrefab = Engine::ImportGLTF(Engine::Files::ASSETS/ "Graphics\\Models\\Cuball.glb", "Cuball_")[0];
        Engine::Transform& transform = ecsSystem->GetComponent<Engine::Transform>(cuballPrefab);
        transform.SetRotation(glm::quat(glm::vec3(glm::radians(90.0f),0,0)));
        transform.SetScale(glm::vec3(0.0f));
        ecsSystem->GetComponent<Engine::MeshRenderer>(ecsSystem->GetEntity(*transform.GetChild(0))).isActive = false;
        for(auto& renderer : Engine::GetComponentsInChildren<Engine::MeshRenderer>(ecsSystem->GetEntity(*transform.GetChild(2))))
            renderer->isActive = false;
    }
    Engine::Entity enemy = CopyEntity(cuballPrefab, true);
    ecsSystem->GetComponent<Engine::Transform>(enemy).SetScale(glm::vec3(1.0f));
    ecsSystem->AddComponent<Engine::BoxCollider>(enemy, Engine::BoxCollider());
    ecsSystem->GetComponent<Engine::BoxCollider>(enemy).size = glm::vec3(0.5f);
    ecsSystem->GetComponent<Engine::BoxCollider>(enemy).layer = static_cast<unsigned char>(CollisionLayer::Enemy);
    EnemyBehaviour behaviour;
    behaviour.behaviour = EnemyBehaviour::Cuball;
    behaviour.startPos = startPos;
    behaviour.speed = Defines::Float("Cuball_Cube_Speed");
    behaviour.bulletSpeed = Defines::Float("Cuball_Cube_BulletSpeed");
    behaviour.spawnTime = Engine::Systems::timeManager->GetTimeSinceStartup();
    behaviour.enemyExtra.cuball = CuballExtra{};
    behaviour.enemyExtra.cuball.rotationParent = ecsSystem->CreateEntity();
    ecsSystem->AddComponent<Engine::Transform>(behaviour.enemyExtra.cuball.rotationParent);
    ecsSystem->AddComponent<EnemyBehaviour>(enemy, behaviour);
    ecsSystem->AddComponent<Health>(enemy, Health{Defines::Int("Cuball_Cube_Health"), Defines::Int("Cuball_Cube_Health")});

    return enemy;
}

Engine::Entity ECSHelper::SpawnDuke(std::pair<int, int> startPos)
{
    if(dukePrefab == Engine::Entity::INVALID_ENTITY_ID)
    {
        dukePrefab = Engine::ImportGLTF(Engine::Files::ASSETS/ "Graphics\\Models\\Duke.glb")[0];
        ecsSystem->GetComponent<Engine::Transform>(dukePrefab).SetRotation(glm::quat(glm::vec3(glm::radians(90.0f),0,0)));
        ecsSystem->GetComponent<Engine::Transform>(dukePrefab).SetScale(glm::vec3(0.0f));
    }
    Engine::Entity enemy = CopyEntity(dukePrefab, true);
    ecsSystem->GetComponent<Engine::Transform>(enemy).SetScale(glm::vec3(1.0f));
    ecsSystem->AddComponent<Engine::BoxCollider>(enemy, Engine::BoxCollider());
    ecsSystem->GetComponent<Engine::BoxCollider>(enemy).size = glm::vec3(0.5f);
    ecsSystem->GetComponent<Engine::BoxCollider>(enemy).layer = static_cast<unsigned char>(CollisionLayer::Enemy);
    EnemyBehaviour behaviour;
    behaviour.behaviour = EnemyBehaviour::Duke;
    behaviour.startPos = startPos;
    behaviour.speed = Defines::Float("Hubertus_Speed");
    behaviour.bulletSpeed = Defines::Float("Hubertus_BulletSpeed");
    behaviour.spawnTime = Engine::Systems::timeManager->GetTimeSinceStartup();
    ecsSystem->AddComponent<EnemyBehaviour>(enemy, behaviour);
    ecsSystem->AddComponent<Health>(enemy, Health{Defines::Int("Hubertus_Health"), Defines::Int("Hubertus_Health")});

    return enemy;
}

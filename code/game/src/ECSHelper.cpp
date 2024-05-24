#include "ECSHelper.h"
#include "Engine.h"
#include "ECSExtension.h"
#include "CollisionLayer.h"
#include "GameDefines.h"

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
    ecsSystem->RegisterComponent<Hubertus>();
    ecsSystem->RegisterComponent<KindredSpirit>();
    ecsSystem->RegisterComponent<Assi>();
    ecsSystem->RegisterComponent<Cuball>();
    ecsSystem->RegisterComponent<Duke>();
    ecsSystem->RegisterComponent<Health>();
    ecsSystem->RegisterComponent<Dungeon>();
    ecsSystem->RegisterComponent<Loot>();
    ecsSystem->RegisterComponent<Elevator>();
    ecsSystem->RegisterComponent<BreakableWall>();
    //When adding new components here, don't forget to add them to ECSHelper::CopyEntity, too!!!!!

    Systems::playerControllerSystem = ecsSystem->RegisterSystem<PlayerControllerSystem>();
    Engine::Signature  playerControllerSignature;
    playerControllerSignature.set(ecsSystem->GetComponentType<Engine::Transform>());
    playerControllerSignature.set(ecsSystem->GetComponentType<PlayerController>());
    playerControllerSignature.set(ecsSystem->GetComponentType<Engine::BoxCollider>());
    playerControllerSignature.set(ecsSystem->GetComponentType<Health>());
    ecsSystem->AddSystemSignature<PlayerControllerSystem>(playerControllerSignature);

    Systems::bulletSystem = ecsSystem->RegisterSystem<BulletSystem>();
    Engine::Signature bulletSignature;
    bulletSignature.set(ecsSystem->GetComponentType<Engine::Transform>());
    bulletSignature.set(ecsSystem->GetComponentType<Bullet>());
    bulletSignature.set(ecsSystem->GetComponentType<Engine::BoxCollider>());
    ecsSystem->AddSystemSignature<BulletSystem>(bulletSignature);

    Systems::enemyBehaviourSystem = ecsSystem->RegisterSystem<EnemyBehaviourSystem>();
    Engine::Signature enemyBehaviourSignature;
    enemyBehaviourSignature.set(ecsSystem->GetComponentType<Engine::Transform>());
    enemyBehaviourSignature.set(ecsSystem->GetComponentType<EnemyBehaviour>());
    enemyBehaviourSignature.set(ecsSystem->GetComponentType<Engine::BoxCollider>());
    enemyBehaviourSignature.set(ecsSystem->GetComponentType<Health>());
    ecsSystem->AddSystemSignature<EnemyBehaviourSystem>(enemyBehaviourSignature);

    Systems::hubertusSystem = ecsSystem->RegisterSystem<HubertusSystem>();
    Engine::Signature hubertusSignature;
    hubertusSignature.set(ecsSystem->GetComponentType<Engine::Transform>());
    hubertusSignature.set(ecsSystem->GetComponentType<EnemyBehaviour>());
    hubertusSignature.set(ecsSystem->GetComponentType<Engine::BoxCollider>());
    hubertusSignature.set(ecsSystem->GetComponentType<Health>());
    hubertusSignature.set(ecsSystem->GetComponentType<Hubertus>());
    ecsSystem->AddSystemSignature<HubertusSystem>(hubertusSignature);

    Systems::kindredSpiritSystem = ecsSystem->RegisterSystem<KindredSpiritSystem>();
    Engine::Signature kindredSpiritSignature;
    kindredSpiritSignature.set(ecsSystem->GetComponentType<Engine::Transform>());
    kindredSpiritSignature.set(ecsSystem->GetComponentType<EnemyBehaviour>());
    kindredSpiritSignature.set(ecsSystem->GetComponentType<Engine::BoxCollider>());
    kindredSpiritSignature.set(ecsSystem->GetComponentType<Health>());
    kindredSpiritSignature.set(ecsSystem->GetComponentType<KindredSpirit>());
    ecsSystem->AddSystemSignature<KindredSpiritSystem>(kindredSpiritSignature);

    Systems::assiSystem = ecsSystem->RegisterSystem<AssiSystem>();
    Engine::Signature assiSignature;
    assiSignature.set(ecsSystem->GetComponentType<Engine::Transform>());
    assiSignature.set(ecsSystem->GetComponentType<EnemyBehaviour>());
    assiSignature.set(ecsSystem->GetComponentType<Engine::BoxCollider>());
    assiSignature.set(ecsSystem->GetComponentType<Health>());
    assiSignature.set(ecsSystem->GetComponentType<Assi>());
    ecsSystem->AddSystemSignature<AssiSystem>(assiSignature);

    Systems::cuballSystem = ecsSystem->RegisterSystem<CuballSystem>();
    Engine::Signature cuballSignature;
    cuballSignature.set(ecsSystem->GetComponentType<Engine::Transform>());
    cuballSignature.set(ecsSystem->GetComponentType<EnemyBehaviour>());
    cuballSignature.set(ecsSystem->GetComponentType<Engine::BoxCollider>());
    cuballSignature.set(ecsSystem->GetComponentType<Health>());
    cuballSignature.set(ecsSystem->GetComponentType<Cuball>());
    ecsSystem->AddSystemSignature<CuballSystem>(cuballSignature);

    Systems::dukeSystem = ecsSystem->RegisterSystem<DukeSystem>();
    Engine::Signature dukeSignature;
    dukeSignature.set(ecsSystem->GetComponentType<Engine::Transform>());
    dukeSignature.set(ecsSystem->GetComponentType<EnemyBehaviour>());
    dukeSignature.set(ecsSystem->GetComponentType<Engine::BoxCollider>());
    dukeSignature.set(ecsSystem->GetComponentType<Health>());
    dukeSignature.set(ecsSystem->GetComponentType<Duke>());
    ecsSystem->AddSystemSignature<DukeSystem>(dukeSignature);

    Systems::dungeonSystem = ecsSystem->RegisterSystem<DungeonSystem>();
    Engine::Signature dungeonSignature;
    dungeonSignature.set(ecsSystem->GetComponentType<Dungeon>());
    ecsSystem->AddSystemSignature<DungeonSystem>(dungeonSignature);

    Systems::elevatorSystem = ecsSystem->RegisterSystem<ElevatorSystem>();
    Engine::Signature elevatorSignature;
    elevatorSignature.set(ecsSystem->GetComponentType<Elevator>());
    ecsSystem->AddSystemSignature<ElevatorSystem>(elevatorSignature);

    Systems::destroyerSystem = ecsSystem->RegisterSystem<BreakableWallSystem>();
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
    if(ecsSystem->HasComponent<EnemyBehaviour>(entity))
        ecsSystem->AddComponent(newEntity, ecsSystem->GetComponent<EnemyBehaviour>(entity));
    if(ecsSystem->HasComponent<Hubertus>(entity))
        ecsSystem->AddComponent(newEntity, ecsSystem->GetComponent<Hubertus>(entity));
    if(ecsSystem->HasComponent<KindredSpirit>(entity))
        ecsSystem->AddComponent(newEntity, ecsSystem->GetComponent<KindredSpirit>(entity));
    if(ecsSystem->HasComponent<Assi>(entity))
        ecsSystem->AddComponent(newEntity, ecsSystem->GetComponent<Assi>(entity));
    if(ecsSystem->HasComponent<Cuball>(entity))
        ecsSystem->AddComponent(newEntity, ecsSystem->GetComponent<Cuball>(entity));
    if(ecsSystem->HasComponent<Duke>(entity))
        ecsSystem->AddComponent(newEntity, ecsSystem->GetComponent<Duke>(entity));

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

Engine::Entity ECSHelper::CreateHubertus(std::pair<int, int> startPos)
{
    if(hubertusPrefab == Engine::Entity::INVALID_ENTITY_ID)
    {
        hubertusPrefab = Engine::ImportGLTF(Engine::Files::ASSETS/ "Graphics\\Models\\Hubertus.glb")[0];
        ecsSystem->GetComponent<Engine::Transform>(hubertusPrefab).SetRotation(glm::quat(glm::vec3(glm::radians(90.0f),0,0)));
        ecsSystem->GetComponent<Engine::Transform>(hubertusPrefab).SetScale(glm::vec3(0.0f));
        numberOfEnemyBehaviourPrefabs++;
    }
    Engine::Entity enemy = CopyEntity(hubertusPrefab, true);
    ecsSystem->GetComponent<Engine::Transform>(enemy).SetScale(glm::vec3(1.0f));
    ecsSystem->AddComponent<Engine::BoxCollider>(enemy, Engine::BoxCollider());
    ecsSystem->GetComponent<Engine::BoxCollider>(enemy).size = glm::vec3(0.9f);
    ecsSystem->GetComponent<Engine::BoxCollider>(enemy).layer = static_cast<unsigned char>(CollisionLayer::Enemy);
    EnemyBehaviour behaviour;
    behaviour.spawnTime = Engine::Systems::timeManager->GetTimeSinceStartup();
    ecsSystem->AddComponent<EnemyBehaviour>(enemy, behaviour);
    Hubertus hubertus;
    hubertus.startPos = startPos;
    hubertus.speed = Defines::Float("Hubertus_Speed");
    hubertus.bulletSpeed = Defines::Float("Hubertus_BulletSpeed");
    ecsSystem->AddComponent(enemy, hubertus);
    ecsSystem->AddComponent<Health>(enemy, Health{Defines::Int("Hubertus_Health"), Defines::Int("Hubertus_Health")});

    return enemy;
}

//This function will spawn one at the given position and one at startPos * -1
std::pair<Engine::Entity, Engine::Entity> ECSHelper::CreateKindredSpirit(std::pair<int, int> startPos)
{
    if(kindredSpiritPrefab == Engine::Entity::INVALID_ENTITY_ID)
    {
        kindredSpiritPrefab = Engine::ImportGLTF(Engine::Files::ASSETS / "Graphics\\Models\\Debug.glb")[0];
        ecsSystem->GetComponent<Engine::Transform>(kindredSpiritPrefab).SetRotation(glm::quat(glm::vec3(glm::radians(90.0f), 0, 0)));
        ecsSystem->GetComponent<Engine::Transform>(kindredSpiritPrefab).SetScale(glm::vec3(0.0f));
        numberOfEnemyBehaviourPrefabs++;
    }

    Engine::Entity enemy1 = CopyEntity(kindredSpiritPrefab, true);
    ecsSystem->GetComponent<Engine::Transform>(enemy1).SetScale(glm::vec3(1.0f));

    ecsSystem->AddComponent<Engine::BoxCollider>(enemy1, Engine::BoxCollider());
    Engine::BoxCollider& coll1 = ecsSystem->GetComponent<Engine::BoxCollider>(enemy1);
    coll1.size = glm::vec3(0.5f);
    coll1.layer = static_cast<unsigned char>(CollisionLayer::Enemy);

    EnemyBehaviour behav1;
    behav1.spawnTime = Engine::Systems::timeManager->GetTimeSinceStartup();
    ecsSystem->AddComponent(enemy1, behav1);
    KindredSpirit kindredSpirit1;
    kindredSpirit1 = KindredSpirit();
    kindredSpirit1.startPos = startPos;
    kindredSpirit1.isMainEntity = true;
    kindredSpirit1.speed = Defines::Float("KindredSpirit_Speed");
    ecsSystem->AddComponent<Health>(enemy1, Health{Defines::Int("KindredSpirit_Health"), Defines::Int("KindredSpirit_Health")});

    Engine::Entity enemy2 = CopyEntity(kindredSpiritPrefab, true);
    ecsSystem->GetComponent<Engine::Transform>(enemy2).SetScale(glm::vec3(1.0f));

    ecsSystem->AddComponent<Engine::BoxCollider>(enemy2, Engine::BoxCollider());
    Engine::BoxCollider& coll2 = ecsSystem->GetComponent<Engine::BoxCollider>(enemy2);
    coll2.size = glm::vec3(0.5f);
    coll2.layer = static_cast<unsigned char>(CollisionLayer::Enemy);

    EnemyBehaviour behav2;
    behav2.spawnTime = Engine::Systems::timeManager->GetTimeSinceStartup();
    KindredSpirit kindredSpirit2;
    ecsSystem->AddComponent(enemy2, behav2);
    kindredSpirit2 = KindredSpirit();
    kindredSpirit2.startPos = startPos; //this value needs to be the same as for the first spirit
    kindredSpirit2.speed = Defines::Float("KindredSpirit_Speed");
    kindredSpirit2.isMainEntity = false;
    kindredSpirit2.other = enemy1;
    ecsSystem->AddComponent(enemy2, Health{Defines::Int("KindredSpirit_Health"), Defines::Int("KindredSpirit_Health")});

    kindredSpirit1.other = enemy2;
    ecsSystem->AddComponent(enemy1, kindredSpirit1);
    ecsSystem->AddComponent(enemy2, kindredSpirit2);


    glm::vec4 colour = KindredSpirit::colours.front();
    KindredSpirit::colours.pop();
    for(Engine::MeshRenderer* renderer : Engine::GetComponentsInChildren<Engine::MeshRenderer>(enemy1))
        for(Engine::MeshRenderer::PrimitiveData &primitive : renderer->primitiveData)
            primitive.material.baseColorFactor = colour;
    for(Engine::MeshRenderer* renderer : Engine::GetComponentsInChildren<Engine::MeshRenderer>(enemy2))
        for(Engine::MeshRenderer::PrimitiveData &primitive : renderer->primitiveData)
            primitive.material.baseColorFactor = colour;

    return {enemy1, enemy2};
}


Engine::Entity ECSHelper::CreateAssi(std::pair<int, int> startPos)
{
    if(assiPrefab == Engine::Entity::INVALID_ENTITY_ID)
    {
        assiPrefab = Engine::ImportGLTF(Engine::Files::ASSETS/ "Graphics\\Models\\Assi.glb")[0];
        ecsSystem->GetComponent<Engine::Transform>(assiPrefab).SetRotation(glm::quat(glm::vec3(glm::radians(90.0f),0,0)));
        ecsSystem->GetComponent<Engine::Transform>(assiPrefab).SetScale(glm::vec3(0.0f));
        numberOfEnemyBehaviourPrefabs++;
    }
    Engine::Entity enemy = CopyEntity(assiPrefab, true);
    ecsSystem->GetComponent<Engine::Transform>(enemy).SetScale(glm::vec3(1.0f));
    ecsSystem->AddComponent<Engine::BoxCollider>(enemy, Engine::BoxCollider());
    ecsSystem->GetComponent<Engine::BoxCollider>(enemy).size = glm::vec3(0.5f);
    ecsSystem->GetComponent<Engine::BoxCollider>(enemy).layer = static_cast<unsigned char>(CollisionLayer::Enemy);
    EnemyBehaviour behaviour;
    behaviour.spawnTime = Engine::Systems::timeManager->GetTimeSinceStartup();
    ecsSystem->AddComponent(enemy, behaviour);
    Assi assi;
    assi = Assi();
    assi.startPos = startPos;
    assi.speed = Defines::Float("Assi_Speed");
    ecsSystem->AddComponent(enemy, assi);
    ecsSystem->AddComponent<Health>(enemy, Health{Defines::Int("Assi_Health"), Defines::Int("Assi_Health")});

    return enemy;
}


Engine::Entity ECSHelper::CreateCuball(std::pair<int, int> startPos)
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
        numberOfEnemyBehaviourPrefabs++;
    }
    Engine::Entity enemy = CopyEntity(cuballPrefab, true);
    ecsSystem->GetComponent<Engine::Transform>(enemy).SetScale(glm::vec3(1.0f));
    ecsSystem->AddComponent<Engine::BoxCollider>(enemy, Engine::BoxCollider());
    ecsSystem->GetComponent<Engine::BoxCollider>(enemy).size = glm::vec3(0.5f);
    ecsSystem->GetComponent<Engine::BoxCollider>(enemy).layer = static_cast<unsigned char>(CollisionLayer::Enemy);
    EnemyBehaviour behaviour;
    behaviour.spawnTime = Engine::Systems::timeManager->GetTimeSinceStartup();
    ecsSystem->AddComponent<EnemyBehaviour>(enemy, behaviour);
    Cuball cuball;
    cuball = Cuball{};
    cuball.startPos = startPos;
    cuball.speed = Defines::Float("Cuball_Cube_Speed");
    cuball.bulletSpeed = Defines::Float("Cuball_Cube_BulletSpeed");
    cuball.rotationParent = ecsSystem->CreateEntity();
    ecsSystem->AddComponent(enemy, cuball);
    ecsSystem->AddComponent<Engine::Transform>(cuball.rotationParent);
    ecsSystem->AddComponent<Health>(enemy, Health{Defines::Int("Cuball_Cube_Health"), Defines::Int("Cuball_Cube_Health")});

    return enemy;
}

Engine::Entity ECSHelper::CreateDuke(std::pair<int, int> startPos)
{
    if(dukePrefab == Engine::Entity::INVALID_ENTITY_ID)
    {
        dukePrefab = Engine::ImportGLTF(Engine::Files::ASSETS/ "Graphics\\Models\\Duke.glb")[0];
        ecsSystem->GetComponent<Engine::Transform>(dukePrefab).SetRotation(glm::quat(glm::vec3(glm::radians(90.0f),0,0)));
        ecsSystem->GetComponent<Engine::Transform>(dukePrefab).SetScale(glm::vec3(0.0f));
        numberOfEnemyBehaviourPrefabs++;
    }
    Engine::Entity enemy = CopyEntity(dukePrefab, true);
    ecsSystem->GetComponent<Engine::Transform>(enemy).SetScale(glm::vec3(1.0f));
    ecsSystem->AddComponent<Engine::BoxCollider>(enemy, Engine::BoxCollider());
    ecsSystem->GetComponent<Engine::BoxCollider>(enemy).size = glm::vec3(0.5f);
    ecsSystem->GetComponent<Engine::BoxCollider>(enemy).layer = static_cast<unsigned char>(CollisionLayer::Enemy);
    EnemyBehaviour behaviour;
    Duke duke;
    duke.speed = Defines::Float("Duke_Speed");
    duke.bulletSpeed = Defines::Float("Duke_BulletSpeed");
    duke.startPos = startPos;
    ecsSystem->AddComponent(enemy, duke);
    behaviour.spawnTime = Engine::Systems::timeManager->GetTimeSinceStartup();
    ecsSystem->AddComponent<EnemyBehaviour>(enemy, behaviour);
    ecsSystem->AddComponent<Health>(enemy, Health{Defines::Int("Duke_Health"), Defines::Int("Duke_Health")});

    return enemy;
}

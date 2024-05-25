#include "ECSHelper.h"
#include "Engine.h"
#include "ECSExtension.h"
#include "CollisionLayer.h"
#include "GameDefines.h"

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
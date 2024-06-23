#include "Systems/CuballSystem.h"
#include "ECSExtension.h"

CuballSystem::CuballSystem()
{

}

void CuballSystem::EntityAdded(Engine::Entity entity)
{
    Cuball& cuball = ecsSystem->GetComponent<Cuball>(entity);
    Engine::Transform& transform = ecsSystem->GetComponent<Engine::Transform>(entity);

    transform.SetTranslation(glm::vec3(Systems::dungeonSystem->ToGlobal(cuball.startPos), 0));

    std::vector<std::pair<int,int>> targetNodes = Systems::enemyBehaviourSystem->FindNodes(cuball.startPos.first, cuball.startPos.second);
    std::pair<int, int> target = targetNodes[rand() % targetNodes.size()];
    Systems::enemyBehaviourSystem->SetTarget(cuball.movement, cuball.startPos, target);
    cuball.movement.currentPos = Systems::dungeonSystem->ToGlobal(cuball.startPos);
}

void CuballSystem::EntityRemoved(Engine::Entity entity)
{

}

void CuballSystem::Update(Engine::Entity entity, float deltaTime)
{
    EnemyBehaviour &behaviour = ecsSystem->GetComponent<EnemyBehaviour>(entity);
    Cuball& cuball = ecsSystem->GetComponent<Cuball>(entity);
    Engine::Transform &transform = ecsSystem->GetComponent<Engine::Transform>(entity);
    std::pair<int, int> dungeonSize = Systems::dungeonSystem->GetDungeonSize();

    if(!ecsSystem->HasComponent<Engine::Animator>(entity))
    {
        behaviour.isActive = true;
        ecsSystem->GetComponent<Engine::BoxCollider>(entity).layer = (int)CollisionLayer::Enemy;
    }
    if(!ecsSystem->HasComponent<Engine::Animator>(entity) && cuball.phase == Cuball::MoveToNewPosition)
    {
        behaviour.isActive = false;
        ecsSystem->GetComponent<Engine::BoxCollider>(entity).layer = (int)CollisionLayer::Ignore;

        std::pair<int, int> pos{-1, -1};
        while(Systems::dungeonSystem->IsWall(pos))
        {
            std::pair<int, int> dungeonPos = Systems::dungeonSystem->ToDungeon(transform.GetTranslation());
            int maxX = Cuball::mirrorDirection.first == 1 ? dungeonSize.first / 2 : dungeonSize.first;
            int maxY = Cuball::mirrorDirection.second == 1 ? dungeonSize.second / 2 : dungeonSize.second;
            int offsetX = dungeonPos.first < maxX && Cuball::mirrorDirection.first == 1 ? maxX : 0;
            int offsetY = dungeonPos.second < maxY && Cuball::mirrorDirection.second == 1 ? maxY : 0;

            pos = {rand() % maxX, rand() % maxY};
            pos.first += offsetX;
            pos.second += offsetY;
        }
        transform.SetTranslation(glm::vec3(Systems::dungeonSystem->ToGlobal(pos), 0));
        Engine::Systems::animationSystem->PlayAnimation(entity, "Cuball_Spawning");
        cuball.phase = Cuball::BecomeBall;
    }
    else if(!ecsSystem->HasComponent<Engine::Animator>(entity) && cuball.phase == Cuball::BecomeBall)
    {
        behaviour.isActive = false;
        ecsSystem->GetComponent<Engine::BoxCollider>(entity).layer = (int)CollisionLayer::Ignore;

        ecsSystem->GetComponent<Engine::MeshRenderer>(ecsSystem->GetEntity(*transform.GetChild(0))).isActive = true;
        ecsSystem->GetComponent<Engine::MeshRenderer>(ecsSystem->GetEntity(*transform.GetChild(1))).isActive = false;
        for(auto& renderer : Engine::GetComponentsInChildren<Engine::MeshRenderer>(ecsSystem->GetEntity(*transform.GetChild(2))))
            renderer->isActive = true;

        behaviour.isActive = false;
        Engine::Systems::animationSystem->PlayAnimation(entity, "Cuball_CubeToBall");
        cuball.phase = Cuball::DisableMeshes;
    }
    else if(!ecsSystem->HasComponent<Engine::Animator>(entity) && cuball.phase == Cuball::DisableMeshes)
    {
        for(auto& renderer : Engine::GetComponentsInChildren<Engine::MeshRenderer>(ecsSystem->GetEntity(*transform.GetChild(2))))
            renderer->isActive = false;

        std::pair<int, int> dungeonPos = Systems::dungeonSystem->ToDungeon(transform.GetGlobalTranslation());
        std::vector<std::pair<int,int>> targetNodes = Systems::enemyBehaviourSystem->FindNodes(dungeonPos.first, dungeonPos.second);
        std::pair<int, int> target = targetNodes[rand() % targetNodes.size()];
        cuball.movement.oldTargetNode = dungeonPos;
        cuball.movement.targetNode = target;
        cuball.movement.targetPos = Systems::dungeonSystem->ToGlobal(cuball.movement.targetNode);
        cuball.movement.currentPos = transform.GetGlobalTranslation();
        cuball.movement.direction = glm::normalize(glm::vec2(cuball.movement.targetPos - cuball.movement.currentPos));

        cuball.phase = Cuball::Ball;
    }

    if(!behaviour.isActive) return;

    MoveCuball(entity, deltaTime);
}

void CuballSystem::HandleDamage(Engine::Entity entity, Engine::Entity other)
{
    //Bullet is already destroying itself, so no need to do it here
    Health& health = ecsSystem->GetComponent<Health>(entity);
    EnemyBehaviour& behaviour = ecsSystem->GetComponent<EnemyBehaviour>(entity);
    Cuball& cuball = ecsSystem->GetComponent<Cuball>(entity);

    if(!behaviour.isActive) return;

    health.health--;
    if(health.health <= 0)
    {
        if(cuball.phase == Cuball::Cube)
        {
            behaviour.isActive = false;
            health.health = Defines::Int("Cuball_Ball_Health");
            health.maxHealth = Defines::Int("Cuball_Ball_Health");
            cuball.bulletSpeed = Defines::Float("Cuball_Ball_BulletSpeed");
            cuball.speed = Defines::Float("Cuball_Ball_Speed");

            Engine::Transform& transform = ecsSystem->GetComponent<Engine::Transform>(entity);
            glm::vec3 pos = transform.GetGlobalTranslation();
            transform.SetParent(nullptr);
            transform.SetRotation(glm::quat({glm::radians(90.0f), 0, 0}));
            transform.SetTranslation(pos);

            ecsSystem->RemoveEntity(cuball.rotationParent);
            cuball.rotationParent = Engine::Entity::INVALID_ENTITY_ID;

            Engine::Animation& cuballSpawning = Engine::Systems::animationSystem->GetAnimation("Cuball_Spawning");
            Engine::Systems::animationSystem->PlayAnimation(entity, "Cuball_Spawning", false, cuballSpawning.endTime, -1);

            cuball.phase = Cuball::MoveToNewPosition;
            return;
        }

        Systems::enemyBehaviourSystem->Kill(entity, EnemyBehaviour::scores[EnemyBehaviour::Cuball]);
    }
}

void CuballSystem::MoveCuball(Engine::Entity entity, float deltaTime)
{
    Cuball& cuball = ecsSystem->GetComponent<Cuball>(entity);
    Engine::Transform &transform = ecsSystem->GetComponent<Engine::Transform>(entity);

    std::pair<int, int> targetNode = cuball.movement.targetNode;
    if(cuball.phase == Cuball::Cube)
    {
        cuball.progress += deltaTime * cuball.speed;
        float easedProgress = cuball.progress * cuball.progress * cuball.progress;
        if (easedProgress >= 1.0f)
            easedProgress = 1.0f;
        ecsSystem->GetComponent<Engine::Transform>(cuball.rotationParent).SetRotation(
                glm::mix(cuball.startRotation, cuball.targetRotation, easedProgress));

        if (easedProgress >= 1.0f)
        {
            Engine::Transform &parent = ecsSystem->GetComponent<Engine::Transform>(cuball.rotationParent);
            glm::quat rot = transform.GetGlobalRotation();
            glm::vec3 pos = transform.GetGlobalTranslation();
            transform.SetParent(nullptr);
            transform.SetRotation(rot);
            transform.SetTranslation(pos);

            if (glm::length(cuball.movement.targetPos - glm::vec2(transform.GetGlobalTranslation())) < cuball.speed * deltaTime * 2)
            {
                std::list<std::pair<int, int>> list = Systems::enemyBehaviourSystem->GetNeighbours(cuball.movement.targetNode);
                auto iter = list.end();
                int i = 0;
                do
                {
                    int index = rand() % list.size();
                    iter = list.begin();
                    std::advance(iter, index);
                    i++;
                } while (i < 5 && *iter == cuball.movement.oldTargetNode);
                Systems::enemyBehaviourSystem->SetTarget(cuball.movement, cuball.movement.targetNode, *iter);
            }

            parent.SetTranslation(transform.GetTranslation() + glm::vec3(cuball.movement.direction * 0.5f, -0.5f));
            parent.SetRotation(glm::identity<glm::quat>());
            transform.SetParent(&parent);
            transform.SetTranslation(-glm::vec3(cuball.movement.direction * 0.5f, -0.5f));

            cuball.startRotation = glm::identity<glm::quat>();
            glm::vec3 euler = {glm::radians(cuball.movement.direction.y * -90), glm::radians(cuball.movement.direction.x * 90), 0};
            cuball.targetRotation = glm::quat(euler);
            cuball.progress = 0;
        }
    }
    else if (cuball.phase == Cuball::Ball)
    {
        glm::vec3 distance = transform.GetGlobalTranslation();
        Systems::enemyBehaviourSystem->MoveRandomly(cuball.movement, cuball.speed * deltaTime);
        transform.SetTranslation(glm::vec3(cuball.movement.currentPos, 0));

        distance = transform.GetGlobalTranslation() - distance;
        //distance in radiance = (distance * 2 * pi) / circumference
        distance = distance * -2.0f * glm::pi<float>() / glm::pi<float>();
        glm::vec3 axis = glm::normalize(glm::cross(glm::vec3(0, 0, -1), distance));
        glm::quat rotation = glm::quat(axis * glm::length(distance)) * transform.GetRotation();
        transform.SetRotation(rotation);
    }

    if(targetNode != cuball.movement.targetNode)
    {
        std::list<std::pair<int, int>> list = Systems::enemyBehaviourSystem->GetNeighbours(targetNode);
        for(std::pair<int, int> node : list)
        {
            glm::vec3 direction = glm::vec3(Systems::dungeonSystem->ToGlobal(node), 0) - transform.GetGlobalTranslation();
            sceneManager->GetActiveScene<Game>().CreateBulletCuball(entity, transform.GetGlobalTranslation(), direction, cuball.bulletSpeed, cuball.phase == Cuball::Ball ? 4 : 2);
        }
    }
}

void CuballSystem::Update(float deltaTime)
{
    for (Engine::Entity entity : entities)
    {
        Update(entity, deltaTime);

        for(auto& collision : ecsSystem->GetComponent<Engine::BoxCollider>(entity).collisions)
        {
            Engine::Entity other = collision.other;
            //If is a bullet and was not shot from an enemy
            if(ecsSystem->HasComponent<Bullet>(other) && !ecsSystem->HasComponent<EnemyBehaviour>(ecsSystem->GetComponent<Bullet>(other).spawner))
                HandleDamage(entity, other);
        }
    }
}

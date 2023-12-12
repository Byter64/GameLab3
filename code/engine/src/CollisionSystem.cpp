#include "CollisionSystem.h"
#include "BoxCollider.h"
#include <cstdlib>

extern Engine::ECSSystem ecsSystem;
namespace Engine
{

    void CollisionSystem::CheckCollisions()
    {
        for(auto entity1 = entities.begin(); entity1 != entities.end(); entity1++)
        {
            if(ecsSystem.GetComponent<BoxCollider>(*entity1).isStatic) continue;

            for(auto entity2 = std::next(entity1); entity2 != entities.end(); entity2++)
            {
                CheckCollision(*entity1, *entity2);
            }
        }
    }

    void CollisionSystem::CheckCollision(Entity entity1, Entity entity2)
    {
        BoxCollider& collider1 = ecsSystem.GetComponent<BoxCollider>(entity1);
        BoxCollider& collider2 = ecsSystem.GetComponent<BoxCollider>(entity2);
        Transform& transform1 = ecsSystem.GetComponent<Transform>(entity1);
        Transform& transform2 = ecsSystem.GetComponent<Transform>(entity2);

        transform1.AddTranslation(collider1.position);
        transform2.AddTranslation(collider2.position);
        glm::vec3 position1 = transform1.GetGlobalTranslation();
        glm::vec3 position2 = transform2.GetGlobalTranslation();
        transform1.AddTranslation(-collider1.position);
        transform2.AddTranslation(-collider2.position);

        glm::vec3 size1 = collider1.size * transform1.GetGlobalScale() * 0.5f; //Size is now half of the actual size
        glm::vec3 size2 = collider2.size * transform1.GetGlobalScale() * 0.5f;

        bool areColliding = true;
        if ( std::abs(position1[0] - position2[0]) > (size1[0] + size2[0]) ) areColliding = false;
        if ( std::abs(position1[1] - position2[1]) > (size1[1] + size2[1]) ) areColliding = false;
        if ( std::abs(position1[2] - position2[2]) > (size1[2] + size2[2]) ) areColliding = false;

        Collision collision = {entity1, entity2};

        if(areColliding == false)
        {
            if(activeCollisions.find(collision) != activeCollisions.end())
            {
                activeCollisions.erase(collision);
                collider1.onCollisionExit(collider1, collider2);
                collider2.onCollisionExit(collider2, collider1);
            }

            return;
        }

        if(activeCollisions.find(collision) != activeCollisions.end())
        {
            collider1.onCollisionStay(collider1, collider2);
            collider2.onCollisionStay(collider2, collider1);
        }
        else
        {
            activeCollisions.insert(collision);
            collider1.onCollisionEnter(collider1, collider2);
            collider2.onCollisionEnter(collider2, collider1);
        }
    }

    bool CollisionSystem::Collision::operator==(const CollisionSystem::Collision &other)
    {
        return (entity1 == other.entity1 && entity2 == other.entity2) ||
                (entity2 == other.entity1 && entity1 == other.entity2);
    }

    bool CollisionSystem::Collision::operator!=(const CollisionSystem::Collision &other)
    {
        return !(*this == other);
    }

    bool CollisionSystem::Collision::operator<(const CollisionSystem::Collision &other) const
    {
        return entity1 < other.entity1 || (entity1 == other.entity1 && entity2 < other.entity2);
    }
} // Engine

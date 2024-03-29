#include "Engine.h"
#include <cstdlib>

namespace Engine
{
    void CollisionSystem::CheckCollision(Entity entity1, Entity entity2)
    {
        BoxCollider& collider1 = ecsSystem->GetComponent<BoxCollider>(entity1);
        BoxCollider& collider2 = ecsSystem->GetComponent<BoxCollider>(entity2);
        Transform& transform1 = ecsSystem->GetComponent<Transform>(entity1);
        Transform& transform2 = ecsSystem->GetComponent<Transform>(entity2);

        transform1.AddTranslation(collider1.position);
        transform2.AddTranslation(collider2.position);
        glm::vec3 position1 = transform1.GetGlobalTranslation();
        glm::vec3 position2 = transform2.GetGlobalTranslation();
        transform1.AddTranslation(-collider1.position);
        transform2.AddTranslation(-collider2.position);

        glm::vec3 size1 = collider1.size * transform1.GetGlobalScale() * 0.5f; //Size is now half of the actual size
        glm::vec3 size2 = collider2.size * transform2.GetGlobalScale() * 0.5f;

        bool areColliding = true;
        if ( std::abs(position1[0] - position2[0]) > (size1[0] + size2[0]) ) areColliding = false;
        if ( std::abs(position1[1] - position2[1]) > (size1[1] + size2[1]) ) areColliding = false;
        if ( std::abs(position1[2] - position2[2]) > (size1[2] + size2[2]) ) areColliding = false;

        Collision collision1 = {entity1, entity2};
        Collision collision2 = {entity2, entity1};

        if(!areColliding && collider1.collisions.find(collision1) != collider1.collisions.end())
        {
            //End collision
            collider1.collisions[collision1] = Collision::State::EXIT;
            collider2.collisions[collision2] = Collision::State::EXIT;
        }
        else if (areColliding && collider1.collisions.find(collision1) == collider1.collisions.end())
        {
            //Start collision
            collider1.collisions[collision1] = Collision::State::ENTER;
            collider2.collisions[collision2] = Collision::State::ENTER;
        }
    }

    void CollisionSystem::CheckCollisions()
    {
        for(Entity entity : entities)
        {
            CleanEnterCollision(entity);
            CleanExitCollision(entity);
        }

        for(auto entity1 = entities.begin(); entity1 != entities.end(); entity1++)
        {
            BoxCollider& collider1 = ecsSystem->GetComponent<BoxCollider>(*entity1);
            if(ecsSystem->GetComponent<BoxCollider>(*entity1).isStatic) continue;

            for(auto entity2 = entities.begin(); entity2 != entities.end(); entity2++)
            {
                BoxCollider& collider2 = ecsSystem->GetComponent<BoxCollider>(*entity2);
                if(ignoredLayers[collider1.layer].count(collider2.layer)) continue;
                if(entity1 == entity2) continue;

                CheckCollision(*entity1, *entity2);
            }
        }
    }

    void CollisionSystem::CleanExitCollision(Entity entity)
    {
        BoxCollider& collider = ecsSystem->GetComponent<BoxCollider>(entity);

        for(auto iter = collider.collisions.begin(); iter != collider.collisions.end();)
        {
            if(iter->second == Collision::State::EXIT)
            {
                iter = collider.collisions.erase(iter);
            }
            else
            {
                iter++;
            }
        }
    }

    void CollisionSystem::CleanEnterCollision(Entity entity)
    {
        BoxCollider& collider = ecsSystem->GetComponent<BoxCollider>(entity);

        for(auto pair : collider.collisions)
        {
            if(pair.second == Collision::State::ENTER)
            {
                pair.second = Collision::State::STAY;
            }
        }
    }

    void CollisionSystem::EntityAdded(Entity entity)
    {

    }

    void CollisionSystem::EntityRemoved(Entity entity)
    {
        BoxCollider &collider = ecsSystem->GetComponent<BoxCollider>(entity);
        for (auto collision: collider.collisions)
        {
            Entity other = collision.first.other;
            Collision col = {other, entity};
            BoxCollider &otherCollider = ecsSystem->GetComponent<Engine::BoxCollider>(other);
            otherCollider.collisions.erase(col);
        }
    }

    void CollisionSystem::SetCollisionBetweenLayers(unsigned char layer1, unsigned char layer2, bool canCollide)
    {
        if(canCollide)
        {
            ignoredLayers[layer1].erase(layer2);
            ignoredLayers[layer2].erase(layer1);
        }
        else
        {
            ignoredLayers[layer1].insert(layer2);
            ignoredLayers[layer2].insert(layer1);
        }
    }

} // Engine

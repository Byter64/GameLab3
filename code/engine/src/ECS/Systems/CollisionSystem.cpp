#include "Engine.h"
#include "ECS/Systems/CollisionSystem.h"

#include <cstdlib>

namespace Engine
{
    void CollisionSystem::CheckCollisions()
    {
        for(Entity entity : entitiesWithBoxColliders)
        {
            BoxCollider& collider = ecsSystem->GetComponent<BoxCollider>(entity);
            CleanEnterCollision(collider);
            CleanExitCollision(collider);
        }

        for(Entity entity : entitiesWithTilemapColliders)
        {
            TilemapCollider& collider = ecsSystem->GetComponent<TilemapCollider>(entity);
            CleanEnterCollision(collider);
            CleanExitCollision(collider);
        }

        for(auto entity1: entitiesWithBoxColliders)
        {
            BoxCollider& collider1 = ecsSystem->GetComponent<BoxCollider>(entity1);
            if(ecsSystem->GetComponent<BoxCollider>(entity1).isStatic) continue;

            for(auto entity2: entitiesWithBoxColliders)
            {
                BoxCollider& collider2 = ecsSystem->GetComponent<BoxCollider>(entity2);
                if(ignoredLayers[collider1.layer].count(collider2.layer)) continue;
                if(entity1 == entity2) continue;

                bool areColliding = CheckCollision(collider1, collider2);
                UpdateCollision(collider1, collider2, areColliding);
            }
        }

        for(auto entity1: entitiesWithBoxColliders)
        {
            BoxCollider& collider1 = ecsSystem->GetComponent<BoxCollider>(entity1);
            if(ecsSystem->GetComponent<BoxCollider>(entity1).isStatic) continue;

            for(auto entity2: entitiesWithTilemapColliders)
            {
                TilemapCollider& collider2 = ecsSystem->GetComponent<TilemapCollider>(entity2);
                if(ignoredLayers[collider1.layer].count(collider2.layer)) continue;

                std::pair<bool, glm::vec3> result = CheckCollision(collider2, collider1);
                UpdateCollision(collider2, collider1, result.first, result.second);
            }
        }
    }

    void CollisionSystem::UpdateCollision(BoxCollider &collider1, BoxCollider &collider2, bool areColliding)
    {
        Collision collision1 = {ecsSystem->GetEntity(collider1), ecsSystem->GetEntity(collider2),
                                ecsSystem->GetComponent<Transform>(ecsSystem->GetEntity(collider1)).GetGlobalTranslation(),
                                ecsSystem->GetComponent<Transform>(ecsSystem->GetEntity(collider2)).GetGlobalTranslation()};
        Collision collision2 = {ecsSystem->GetEntity(collider2), ecsSystem->GetEntity(collider1),
                                ecsSystem->GetComponent<Transform>(ecsSystem->GetEntity(collider2)).GetGlobalTranslation(),
                                ecsSystem->GetComponent<Transform>(ecsSystem->GetEntity(collider1)).GetGlobalTranslation()};

        if(!areColliding)
        {
            auto iter1 = std::find(collider1.collisions.begin(), collider1.collisions.end(), collision1);
            auto iter2 = std::find(collider2.collisions.begin(), collider2.collisions.end(), collision1);
            if(iter1 != collider1.collisions.end())
            {
                //End collision
                iter1->state = Collision::State::EXIT;
                iter2->state = Collision::State::EXIT;
            }
        }
        else if (areColliding)
        {
            auto iter1 = std::find(collider1.collisions.begin(), collider1.collisions.end(), collision1);
            auto iter2 = std::find(collider2.collisions.begin(), collider2.collisions.end(), collision1);
            if(iter1 == collider1.collisions.end())
            {
                //Start collision
                iter1->state = Collision::State::ENTER;
                iter2->state = Collision::State::ENTER;
            }
        }
    }

    void CollisionSystem::UpdateCollision(TilemapCollider &collider1, BoxCollider &collider2, bool areColliding, glm::vec3 tilePos)
    {
        Collision collision1 = {ecsSystem->GetEntity(collider1), ecsSystem->GetEntity(collider2),
                                tilePos,
                                ecsSystem->GetComponent<Transform>(ecsSystem->GetEntity(collider2)).GetGlobalTranslation()};
        Collision collision2 = {ecsSystem->GetEntity(collider2), ecsSystem->GetEntity(collider1),
                                ecsSystem->GetComponent<Transform>(ecsSystem->GetEntity(collider2)).GetGlobalTranslation(),
                                tilePos};

        if(!areColliding)
        {
            auto iter1 = std::find(collider1.collisions.begin(), collider1.collisions.end(), collision1);
            auto iter2 = std::find(collider2.collisions.begin(), collider2.collisions.end(), collision2);
            if(iter1 != collider1.collisions.end())
            {
                //End collision
                iter1->state = Collision::State::EXIT;
                iter2->state = Collision::State::EXIT;
            }
        }
        else if (areColliding)
        {
            auto iter1 = std::find(collider1.collisions.begin(), collider1.collisions.end(), collision1);
            auto iter2 = std::find(collider2.collisions.begin(), collider2.collisions.end(), collision2);
            if(iter1 == collider1.collisions.end())
            {
                //Start collision
                collision1.state = Collision::ENTER;
                collision2.state = Collision::ENTER;
                collider1.collisions.push_back(collision1);
                collider2.collisions.push_back(collision2);
            }
            else
            {
                //continue collision with updated position
                iter1->entityPos = tilePos;
                iter2->otherPos = tilePos;
            }
        }
    }

    /// return, if the two given Colliders collide
    /// \param collider1
    /// \param collider2
    /// \return
    bool CollisionSystem::CheckCollision(const BoxCollider &collider1, const BoxCollider &collider2)
    {
        Transform& transform1 = ecsSystem->GetComponent<Transform>(ecsSystem->GetEntity(collider1));
        Transform& transform2 = ecsSystem->GetComponent<Transform>(ecsSystem->GetEntity(collider2));

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

        return areColliding;
    }

    std::pair<bool, glm::vec3> CollisionSystem::CheckCollision(const TilemapCollider &collider1, const BoxCollider &collider2)
    {
        Transform& transform1 = ecsSystem->GetComponent<Transform>(ecsSystem->GetEntity(collider1));
        Transform& transform2 = ecsSystem->GetComponent<Transform>(ecsSystem->GetEntity(collider2));

        transform1.AddTranslation(collider1.position);
        transform2.AddTranslation(collider2.position);
        glm::vec3 position1 = transform1.GetGlobalTranslation();
        glm::vec3 position2 = transform2.GetGlobalTranslation();
        transform1.AddTranslation(-collider1.position);
        transform2.AddTranslation(-collider2.position);

        glm::vec3 distance = position2 - position1;
        std::pair<int, int> origin = {(int)std::round(distance.x), (int)std::round(distance.y)};
        int xtol = 3;
        int ytol = 3;

        for(int x = origin.first - xtol < 0 ? 0 : origin.first - xtol; x < origin.first + xtol && x < collider1.map.size(); x++)
            for(int y = origin.second - ytol < 0 ? 0 : origin.second - ytol; y < origin.second + ytol && y < collider1.map[0].size(); y++)
            {
                if(!collider1.map[x][y]) continue;

                glm::vec3 size1 = glm::vec3(0.5f); //a box of a tilemap always has a radius of 0.5f
                glm::vec3 size2 = collider2.size * transform2.GetGlobalScale() * 0.5f;
                glm::vec3 tilePosition = position1 + glm::vec3(x, y, 0);

                bool areColliding = true;
                if ( std::abs(tilePosition[0] - position2[0]) > (size1[0] + size2[0]) ) areColliding = false;
                if ( std::abs(tilePosition[1] - position2[1]) > (size1[1] + size2[1]) ) areColliding = false;
                if ( std::abs(tilePosition[2] - position2[2]) > (size1[2] + size2[2]) ) areColliding = false;

                if(areColliding)
                    return {true,tilePosition};
            }
        return {false, glm::vec3(0)};
    }

    void CollisionSystem::CleanExitCollision(BoxCollider &collider)
    {
        for(auto iter = collider.collisions.begin(); iter != collider.collisions.end();)
            if(iter->state == Collision::State::EXIT)
                iter = collider.collisions.erase(iter);
            else
                iter++;
    }

    void CollisionSystem::CleanExitCollision(TilemapCollider &collider)
    {
        for(auto iter = collider.collisions.begin(); iter != collider.collisions.end();)
            if(iter->state == Collision::State::EXIT)
                iter = collider.collisions.erase(iter);
            else
                iter++;
    }

    void CollisionSystem::CleanEnterCollision(BoxCollider& collider)
    {
        for(auto& collision : collider.collisions)
            if(collision.state == Collision::State::ENTER)
                collision.state = Collision::State::STAY;
    }

    void CollisionSystem::CleanEnterCollision(TilemapCollider& collider)
    {
        for(auto& collision : collider.collisions)
            if(collision.state == Collision::State::ENTER)
                collision.state = Collision::State::STAY;
    }

    void CollisionSystem::EntityAdded(Entity entity)
    {
        if(ecsSystem->HasComponent<BoxCollider>(entity))
            entitiesWithBoxColliders.insert(entity);
        else if(ecsSystem->HasComponent<TilemapCollider>(entity))
            entitiesWithTilemapColliders.insert(entity);
    }

    void CollisionSystem::EntityRemoved(Entity entity)
    {
        std::vector<Collision> const &collisions = entitiesWithBoxColliders.count(entity) ?
                ecsSystem->GetComponent<BoxCollider>(entity).collisions : ecsSystem->GetComponent<TilemapCollider>(entity).collisions;

        entitiesWithBoxColliders.erase(entity);
        entitiesWithTilemapColliders.erase(entity);

        for (auto& collision: collisions)
        {
            Entity other = collision.other;
            Collision col = {other, entity};
            if (ecsSystem->HasComponent<BoxCollider>(other))
            {
                BoxCollider &otherCollider = ecsSystem->GetComponent<BoxCollider>(other);
                auto iter = std::find(otherCollider.collisions.begin(), otherCollider.collisions.end(), col);
                otherCollider.collisions.erase(iter);
            }
            else if (ecsSystem->HasComponent<TilemapCollider>(other))
            {
                TilemapCollider &otherCollider = ecsSystem->GetComponent<TilemapCollider>(other);
                auto iter = std::find(otherCollider.collisions.begin(), otherCollider.collisions.end(), col);
                otherCollider.collisions.erase(iter);
            }
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

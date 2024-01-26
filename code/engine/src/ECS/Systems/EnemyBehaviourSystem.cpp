#include "ECS/Systems/EnemyBehaviourSystem.h"
#include "Engine.h"
#include <chrono>
#include <iostream>
#include <list>
#include <cstdlib>
#include <iterator>

namespace Engine
{
    EnemyBehaviourSystem::EnemyBehaviourSystem()
    {
        srand(std::chrono::system_clock::now().time_since_epoch().count());
    }

    void EnemyBehaviourSystem::EntityAdded(Entity entity)
    {

    }

    void EnemyBehaviourSystem::EntityRemoved(Entity entity)
    {

    }


    void EnemyBehaviourSystem::Update(float deltaTime)
    {
        //Each entity must have an "EnemyBehaviour", "Health" and "Collider"
        //Wie weiß der Gegner, wo er ist??
        for (Entity entity : entities)
        {
            EnemyBehaviour& behaviour = ecsSystem->GetComponent<EnemyBehaviour>(entity);

            switch (behaviour.behaviour)
            {
                default:
                case EnemyBehaviour::Default:
                    Update1(entity, deltaTime);
                    break;
            }
        }
    }

    void EnemyBehaviourSystem::Update1(Entity entity, float deltaTime)
    {
        EnemyBehaviour& behaviour = ecsSystem->GetComponent<EnemyBehaviour>(entity);
        Transform& transform = ecsSystem->GetComponent<Transform>(entity);

        behaviour.time -= deltaTime;

        switch (behaviour.state)
        {
            case EnemyBehaviour::Spawning:
            {
                if (behaviour.time > 0) return;

                behaviour.state = EnemyBehaviour::Moving;
                behaviour.time = ((rand() / (float) RAND_MAX) * 4 + 1);

                int x = (int) transform.GetGlobalTranslation().x;
                int y = (int) transform.GetGlobalTranslation().y;
                std::pair<int, int> freeAdjacentField = GetShuffeldAdjacentFields(x, y).front();

                behaviour.movement.x = freeAdjacentField.first - x;
                behaviour.movement.y = freeAdjacentField.second - y;
                break;
            }
            case EnemyBehaviour::Idling:
            {
                if (behaviour.time > 0.0f) return;

                unsigned int random = rand() % 10;
                if (random == 0)
                {
                    behaviour.state = EnemyBehaviour::Shooting;
                    behaviour.time = random == 0;
                } else
                {
                    behaviour.state = EnemyBehaviour::Moving;
                    behaviour.time = ((rand() / (float) RAND_MAX) * 4 + 1);

                    int x = (int) transform.GetGlobalTranslation().x;
                    int y = (int) transform.GetGlobalTranslation().y;
                    std::pair<int, int> freeAdjacentField = GetShuffeldAdjacentFields(x, y).front();

                    behaviour.movement.x = freeAdjacentField.first - x;
                    behaviour.movement.y = freeAdjacentField.second - y;
                }
                break;
            }
            case EnemyBehaviour::Moving:
            {
                if (behaviour.time < 0)
                {
                    int number = rand() % 10;

                    if (number < 5)
                    {
                        behaviour.state = EnemyBehaviour::Shooting;
                    }
                    else if (number < 8)
                    {
                        behaviour.state = EnemyBehaviour::Moving;
                        behaviour.time = ((rand() / (float) RAND_MAX) * 4 + 1);

                        int x = (int) transform.GetGlobalTranslation().x;
                        int y = (int) transform.GetGlobalTranslation().y;
                        std::pair<int, int> freeAdjacentField = GetShuffeldAdjacentFields(x, y).front();

                        behaviour.movement.x = freeAdjacentField.first - x;
                        behaviour.movement.y = freeAdjacentField.second - y;
                    }
                    else
                    {
                        behaviour.state = EnemyBehaviour::Idling;
                        behaviour.time = ((rand() / (float) RAND_MAX) * 3 + 1);
                    }

                    return;
                }

                transform.AddTranslation(glm::vec3(glm::normalize(behaviour.movement), 0) * deltaTime * behaviour.movementSpeed);
                int xPos = (int) (transform.GetGlobalTranslation().x + behaviour.movement.x);
                int yPos = (int) (transform.GetGlobalTranslation().y + behaviour.movement.y);

                if (!DoesPositionExistAndIsFree(xPos, yPos))
                {
                    int number = rand() % 10;

                    if (number < 9)
                    {
                        behaviour.state = EnemyBehaviour::Moving;
                        behaviour.time = ((rand() / (float) RAND_MAX) * 4 + 1);

                        int x = (int) transform.GetGlobalTranslation().x;
                        int y = (int) transform.GetGlobalTranslation().y;
                        std::pair<int, int> freeAdjacentField = GetShuffeldAdjacentFields(x, y).front();

                        behaviour.movement.x = freeAdjacentField.first - x;
                        behaviour.movement.y = freeAdjacentField.second - y;
                    }
                    else
                    {
                        behaviour.state = EnemyBehaviour::Idling;
                        behaviour.time = ((rand() / (float) RAND_MAX) * 3 + 1);
                    }
                }
                break;
            }
            case EnemyBehaviour::Shooting:
            {
                std::cout << "I shoot" << std::endl;
                unsigned int random = rand() % 10;
                if (random == 0)
                {
                    behaviour.state = EnemyBehaviour::Shooting;
                    behaviour.time = random == 0;
                } else
                {
                    behaviour.state = EnemyBehaviour::Moving;
                    behaviour.time = ((rand() / (float) RAND_MAX) * 4 + 1);

                    int x = (int) transform.GetGlobalTranslation().x;
                    int y = (int) transform.GetGlobalTranslation().y;
                    std::pair<int, int> freeAdjacentField = GetShuffeldAdjacentFields(x, y).front();

                    behaviour.movement.x = freeAdjacentField.first - x;
                    behaviour.movement.y = freeAdjacentField.second - y;
                }
                break;
            }
        }
    }

    bool EnemyBehaviourSystem::DoesPositionExistAndIsFree(int x, int y)
    {
        x += dungeonMap.size() / 2;
        y += dungeonMap[0].size() / 2;

        if (x < 0 || x >= dungeonMap.size() || y < 0 || y >= dungeonMap[0].size())
        {
            return false;
        }

        return !dungeonMap[x][y];
    }

    std::queue<std::pair<int, int>> EnemyBehaviourSystem::GetShuffeldAdjacentFields(int x, int y)
    {
        std::queue<std::pair<int, int>> adjacentFields;
        std::list<int> numbers{ 1,2,3,4 };

        while (!numbers.empty())
        {
            int index = rand() % numbers.size();
            auto iter = numbers.begin();
            std::advance(iter, index);
            switch (*iter)
            {
                case 1:
                    if (DoesPositionExistAndIsFree(x + 1, y))
                        adjacentFields.push({ x + 1, y });
                    break;
                case 2:
                    if (DoesPositionExistAndIsFree(x, y + 1))
                        adjacentFields.push({ x, y + 1});
                    break;
                case 3:
                    if (DoesPositionExistAndIsFree(x - 1, y))
                        adjacentFields.push({ x - 1, y });
                    break;
                case 4:
                    if (DoesPositionExistAndIsFree(x, y - 1))
                        adjacentFields.push({ x, y - 1});
                    break;
            }

            iter = numbers.begin();
            std::advance(iter, index);
            numbers.remove(*iter);
        }

        return adjacentFields;
    }
}
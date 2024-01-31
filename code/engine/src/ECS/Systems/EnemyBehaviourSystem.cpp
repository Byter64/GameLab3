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
        for (Entity entity: entities)
        {
            EnemyBehaviour &behaviour = ecsSystem->GetComponent<EnemyBehaviour>(entity);

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
        EnemyBehaviour &behaviour = ecsSystem->GetComponent<EnemyBehaviour>(entity);
        Transform &transform = ecsSystem->GetComponent<Transform>(entity);

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
                    } else if (number < 8)
                    {
                        behaviour.state = EnemyBehaviour::Moving;
                        behaviour.time = ((rand() / (float) RAND_MAX) * 4 + 1);

                        int x = (int) transform.GetGlobalTranslation().x;
                        int y = (int) transform.GetGlobalTranslation().y;
                        std::pair<int, int> freeAdjacentField = GetShuffeldAdjacentFields(x, y).front();

                        behaviour.movement.x = freeAdjacentField.first - x;
                        behaviour.movement.y = freeAdjacentField.second - y;
                    } else
                    {
                        behaviour.state = EnemyBehaviour::Idling;
                        behaviour.time = ((rand() / (float) RAND_MAX) * 3 + 1);
                    }

                    return;
                }

                transform.AddTranslation(
                        glm::vec3(glm::normalize(behaviour.movement), 0) * deltaTime * behaviour.movementSpeed);
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
                    } else
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
        x += wallMap.size() / 2;
        y += wallMap[0].size() / 2;

        if (x < 0 || x >= wallMap.size() || y < 0 || y >= wallMap[0].size())
        {
            return false;
        }

        return !wallMap[x][y];
    }

    std::queue<std::pair<int, int>> EnemyBehaviourSystem::GetShuffeldAdjacentFields(int x, int y)
    {
        std::queue<std::pair<int, int>> adjacentFields;
        std::list<int> numbers{1, 2, 3, 4};

        while (!numbers.empty())
        {
            int index = rand() % numbers.size();
            auto iter = numbers.begin();
            std::advance(iter, index);
            switch (*iter)
            {
                case 1:
                    if (DoesPositionExistAndIsFree(x + 1, y))
                        adjacentFields.push({x + 1, y});
                    break;
                case 2:
                    if (DoesPositionExistAndIsFree(x, y + 1))
                        adjacentFields.push({x, y + 1});
                    break;
                case 3:
                    if (DoesPositionExistAndIsFree(x - 1, y))
                        adjacentFields.push({x - 1, y});
                    break;
                case 4:
                    if (DoesPositionExistAndIsFree(x, y - 1))
                        adjacentFields.push({x, y - 1});
                    break;
            }

            iter = numbers.begin();
            std::advance(iter, index);
            numbers.remove(*iter);
        }

        return adjacentFields;
    }

    std::pair<int, int> EnemyBehaviourSystem::FindNode(int startx, int starty, int dirx, int diry)
    {
        startx += dirx;
        starty += diry;
        while(startx < dungeonSize.first && startx >= 0 && starty < dungeonSize.second && starty >= 0)
        {
            if(graph.count(std::make_pair(startx, starty)))
            {
                return std::make_pair(startx, starty);
            }

            startx += dirx;
            starty += diry;
        }
        return std::make_pair(-1, -1);
    }

    /// This needs to be called before the first call to Update
    /// \param wallMap a bool map of the dungeon, where true marks a wall, a false marks free space
    void EnemyBehaviourSystem::Initialize(std::vector<std::vector<bool>>& wallMap)
    {
        dungeonSize = std::make_pair(wallMap.size(), wallMap[0].size());
        //Generate all nodes
        for (int y = 0; y < wallMap[0].size(); y++)
        {
            for (int x = 0; x < wallMap.size(); x++)
            {
                if (IsNode(wallMap, x, y))
                {
                    graph[std::make_pair(x, y)] = std::list<std::pair<int, int>>();
                }
            }
        }

        //Create connections between nodes
        for (auto pair : graph)
        {
            std::pair<int, int> pos = pair.first;
            std::pair<int, int> top = FindNode(pos.first, pos.second, 0, 1);
            std::pair<int, int> bottom = FindNode(pos.first, pos.second, 0, -1);
            std::pair<int, int> left = FindNode(pos.first, pos.second, -1, 0);
            std::pair<int, int> right = FindNode(pos.first, pos.second, 1, 0);

            if(top.first != -1)
                pair.second.push_back(top);
            if(bottom.first != -1)
                pair.second.push_back(bottom);
            if(left.first != -1)
                pair.second.push_back(left);
            if(right.first != -1)
                pair.second.push_back(right);
        }
    }

    /// Will crash, if the wallMap does not have a wall on its outer border
    bool EnemyBehaviourSystem::IsNode(std::vector<std::vector<bool>> &wallMap, int x, int y)
    {
        //position is a wall
        if(wallMap[x][y]) return false;

        int connections = 0;
        if(!wallMap[x + 1][y]) connections++;
        if(!wallMap[x - 1][y]) connections++;
        if(!wallMap[x][y + 1]) connections++;
        if(!wallMap[x][y - 1]) connections++;

        //position is not a straight path
        if(connections != 2) return true;

        //position is a straight path
        if((!wallMap[x + 1][y] && !wallMap[x - 1][y]) || (!wallMap[x][y + 1] && !wallMap[x][y - 1])) return false;

        //position is a corner
        return true;
    }
}
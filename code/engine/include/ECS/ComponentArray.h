#pragma once
#include "IComponentArray.h"
#include <array>
#include <unordered_map>
#include <stdexcept>
#include <string>
#include "Entity.h"
#include <iostream>

namespace Engine
{
    template<typename T>
    class ComponentArray : public IComponentArray
    {
        size_t size = 0;
        std::array<T, Entity::MAX_ENTITIES> components;
        std::unordered_map<Entity, size_t> entityToIndex;
        std::unordered_map<size_t, Entity> indexToEntity;

    public:
        //Component data is undefined, when using this AddComponent overload!!!
        T& AddComponent(Entity entity)
        {
            if(entityToIndex.find(entity) != entityToIndex.end())
            {
                std::string message{"A component of type \""};
                message += typeid(T).name();
                message += "has already been added to entity ";
                message += std::to_string(entity.id);

                throw std::runtime_error(message);
            }

            size_t index = size;
            entityToIndex[entity] = index;
            indexToEntity[index] = entity;
            size++;

            return components[index];
        }

        void AddComponent(Entity entity, T component)
        {
            if(entityToIndex.find(entity) != entityToIndex.end())
            {
                std::string message{"A component of type \""};
                message += typeid(T).name();
                message += "has already been added to entity ";
                message += std::to_string(entity.id);

                throw std::runtime_error(message);
            }
            size_t index = size;
            entityToIndex[entity] = index;
            indexToEntity[index] = entity;
            components[index] = component;
            size++;
        }

        void RemoveComponent(Entity entity)
        {
            if(entityToIndex.find(entity) == entityToIndex.end())
            {
                std::string message{"The entity "};
                message += std::to_string(entity.id);
                message += " does not have a component of type \"";
                message += typeid(T).name();
                message += "\"";

                throw std::runtime_error(message);
            }
            size_t entityIndex = entityToIndex[entity];
            size_t lastIndex = size - 1;
            components[entityIndex] = components[lastIndex];

            Entity lastEntity = indexToEntity[lastIndex];
            entityToIndex[lastEntity] = entityIndex;
            indexToEntity[entityIndex] = lastEntity;

            entityToIndex.erase(entity);
            indexToEntity.erase(lastIndex);

            size--;
        }

        T& GetComponent(Entity entity)
        {
            if(entityToIndex.find(entity) == entityToIndex.end())
            {
                std::string message{"The entity "};
                message += std::to_string(entity.id);
                message += " does not have a component of type \"";
                message += typeid(T).name();
                message += "\"";

                throw std::runtime_error(message);
            }

            return components[entityToIndex[entity]];
        }

        bool HasComponent(Entity entity)
        {
            return entityToIndex.find(entity) != entityToIndex.end();
        }

        Entity GetEntity(T& component)
        {
            T* firstElement = &components[0];
            T* thisElement = &component;
            int index = thisElement - firstElement;
            return indexToEntity[index];
        }

        void EntityDestroyed(Entity entity) override
        {
            if(entityToIndex.find(entity) != entityToIndex.end())
            {
                RemoveComponent(entity);
            }
        }
    };

} // Engine

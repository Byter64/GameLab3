#pragma once
#include "IComponentArray.h"
#include <array>
#include <unordered_map>
#include <stdexcept>
#include <string>
#include "Entity.h"
#include <iostream>
#include "ECSException.h"

namespace Engine
{
    template<typename T>
    class ComponentArray : public IComponentArray
    {
        size_t size = 0;
        std::array<T, Entity::MAX_ENTITIES> components;
        std::unordered_map<std::uint32_t, size_t> entityToIndex;
        std::unordered_map<size_t, std::uint32_t> indexToEntity;

    public:
        //Component data is default initialized, when using this AddComponent overload
        T& AddComponent(Entity entity)
        {
            if(entityToIndex.find(entity.id) != entityToIndex.end())
            {
                std::string message{"A component of type \""};
                message += typeid(T).name();
                message += "\" has already been added to entity ";
                message += std::to_string(entity.id);

                throw ECSException("COMPONENT ALREADY EXISTS", message, entity);
            }

            size_t index = size;
            entityToIndex[entity.id] = index;
            indexToEntity[index] = entity.id;
            components[index] = T();
            size++;

            return components[index];
        }

        void AddComponent(Entity entity, T component)
        {
            if(entityToIndex.find(entity.id) != entityToIndex.end())
            {
                std::string message{"A component of type \""};
                message += typeid(T).name();
                message += "\" has already been added to entity ";
                message += std::to_string(entity.id);

                throw ECSException("COMPONENT ALREADY EXISTS", message, entity);
            }
            size_t index = size;
            entityToIndex[entity.id] = index;
            indexToEntity[index] = entity.id;
            components[index] = component;
            size++;
        }

        void RemoveComponent(Entity entity)
        {
            if(entityToIndex.find(entity.id) == entityToIndex.end())
            {
                std::string message{"The entity "};
                message += std::to_string(entity.id);
                message += " does not have a component of type \"";
                message += typeid(T).name();
                message += "\"";

                throw ECSException("COMPONENT NOT FOUND", message, entity);
            }
            size_t entityIndex = entityToIndex[entity.id];
            size_t lastIndex = size - 1;
            components[entityIndex] = std::move(components[lastIndex]);

            Entity lastEntity = indexToEntity[lastIndex];
            entityToIndex[lastEntity.id] = entityIndex;
            indexToEntity[entityIndex] = lastEntity.id;

            entityToIndex.erase(entity.id);
            indexToEntity.erase(lastIndex);

            size--;
        }

        T& GetComponent(Entity entity)
        {
            if(entityToIndex.find(entity.id) == entityToIndex.end())
            {
                std::string message{"The entity "};
                message += std::to_string(entity.id);
                message += " does not have a component of type \"";
                message += typeid(T).name();
                message += "\"";

                throw ECSException("COMPONENT NOT FOUND", message, entity);
            }

            return components[entityToIndex[entity.id]];
        }

        bool HasComponent(Entity entity)
        {
            return entityToIndex.find(entity.id) != entityToIndex.end();
        }

        int GetNumberOfComponents()
        {
            return size;
        }

        Entity GetEntity(T const& component)
        {
            T* firstElement = &components[0];
            const T* thisElement = &component;
            int index = thisElement - firstElement;
            return indexToEntity.at(index);
        }

        void EntityDestroyed(Entity entity) override
        {
            if(entityToIndex.find(entity.id) != entityToIndex.end())
            {
                RemoveComponent(entity);
            }
        }
    };

} // Engine

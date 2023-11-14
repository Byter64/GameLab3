#pragma once
#include "IComponentArray.h"
#include <array>
#include <unordered_map>
#include <assert.h>
#include "Entity.h"

namespace Engine
{
    template<typename T>
    class ComponentArray : public IComponentArray
    {
        size_t size = 0;
        std::array<T, MAX_ENTITIES> components;
        std::unordered_map<Entity, size_t> entityToIndex;
        std::unordered_map<size_t, Entity> indexToEntity;

    public:
        void AddComponent(Entity entity, T component)
        {
            assert(entityToIndex.find(entity) == entityToIndex.end() && "This type of component has already been added to this entity");

            size_t index = size;
            entityToIndex[entity] = index;
            indexToEntity[index] = entity;
            components[index] = component;
            size++;
        }

        void RemoveComponent(Entity entity)
        {
            assert(entityToIndex.find(entity) != entityToIndex.end() && "This entity does not have a component of this type");

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

        /**
         * Returns a reference to the component of the given entity.
         * @param entity
         * @return a reference to the entitie's component or nullptr if the entity does not have one
         */
        T& GetComponent(Entity entity)
        {
            bool entityHasComponent = entityToIndex.find(entity) != entityToIndex.end();
            if(entityHasComponent)
                return components[entityToIndex[entity]];
            else
                return nullptr;
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

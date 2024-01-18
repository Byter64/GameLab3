#pragma once
#include <cstdint>
#include <bitset>
#include "ComponentType.h"
#include <iostream>

namespace Engine
{
    //typedef std::uint32_t Entity;
    typedef std::bitset<MAX_COMPONENTS> Signature;
    struct Entity
    {
        static const std::uint32_t MAX_ENTITIES = 10000;
        static const std::uint32_t INVALID_ENTITY_ID = MAX_ENTITIES;

        std::uint32_t id;

        Entity() = default;
        Entity(int id);
        Entity(std::uint32_t id);

        Entity operator+(const Entity& other) const;
        Entity operator-(const Entity& other) const;
        Entity operator*(const Entity& other) const;
        Entity operator/(const Entity& other) const;
        Entity operator%(const Entity& other) const;
        Entity operator^(const Entity& other) const;
        Entity operator&(const Entity& other) const;
        Entity operator|(const Entity& other) const;
        Entity operator~() const;
        Entity operator!() const;
        Entity operator<<(const Entity& other) const;
        Entity operator>>(const Entity& other) const;
        bool operator==(const Entity& other) const;
        bool operator!=(const Entity& other) const;
        bool operator<(const Entity& other) const;
        bool operator>(const Entity& other) const;
        bool operator&&(const Entity& other) const;
        bool operator||(const Entity& other) const;
    };
    std::ostream& operator<<(std::ostream& os, const Entity& entity);
} // Engine

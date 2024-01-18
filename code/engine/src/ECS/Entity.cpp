#include "ECS/Entity.h"

namespace Engine
{
    bool Entity::operator==(const Entity &other) const
    {
        return id == other.id;
    }

    bool Entity::operator!=(const Entity &other) const
    {
        return !(*this == other);
    }

    bool Entity::operator<(const Entity &other) const
    {
        return id < other.id;
    }

    bool Entity::operator>(const Entity &other) const
    {
        return id > other.id;
    }

    Entity Entity::operator+(const Entity &other) const
    {
        return Entity{id + other.id};
    }

    Entity Entity::operator-(const Entity &other) const
    {
        return Entity{id - other.id};
    }

    Entity Entity::operator*(const Entity &other) const
    {
        return Entity{id * other.id};
    }

    Entity Entity::operator/(const Entity &other) const
    {
        return Entity{id * other.id};
    }

    Entity Entity::operator%(const Entity &other) const
    {
        return Entity{id % other.id};
    }

    Entity Entity::operator^(const Entity &other) const
    {
        return Entity{id ^ other.id};
    }

    Entity Entity::operator&(const Entity &other) const
    {
        return Entity{id & other.id};
    }

    Entity Entity::operator|(const Entity &other) const
    {
        return Entity{id | other.id};
    }

    Entity Entity::operator~() const
    {
        return Entity{~id};
    }

    Entity Entity::operator!() const
    {
        return Entity{!id};
    }

    Entity Entity::operator<<(const Entity &other) const
    {
        return Entity{id << other.id};
    }

    Entity Entity::operator>>(const Entity &other) const
    {
        return Entity{id >> other.id};
    }

    bool Entity::operator&&(const Entity &other) const
    {
        return id && other.id;
    }

    bool Entity::operator||(const Entity &other) const
    {
        return id || other.id;
    }

    Entity::Entity(int id) : id(id) { }

    Entity::Entity(std::uint32_t id) : id(id) { }

    std::ostream& operator<<(std::ostream& os, const Entity& entity)
    {
        return (os << entity.id);
    }
}
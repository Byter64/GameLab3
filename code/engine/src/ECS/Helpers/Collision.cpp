#include "ECS/Helpers/Collision.h"

namespace Engine
{
    bool Collision::operator==(const Collision &other) const
    {
        return (entity == other.entity && this->other == other.other);
    }

    bool Collision::operator!=(const Collision &other) const
    {
        return !(*this == other);
    }

    bool Collision::operator<(const Collision &other) const
    {
        return entity < other.entity || (entity == other.entity && this->other < other.other);
    }
}
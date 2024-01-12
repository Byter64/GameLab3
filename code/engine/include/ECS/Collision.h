#pragma once
#include "Entity.h"
namespace Engine
{

    struct Collision
    {
        enum State
        {
            ENTER,
            STAY,
            EXIT
        };

        Entity entity;
        Entity other;

        Collision() = default;

        bool operator==(const Collision &other) const;
        bool operator!=(const Collision &other) const;
        bool operator<(const Collision &other) const;
    };

} // Engine

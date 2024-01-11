#pragma once
#include <cstdint>
#include <bitset>
#include "ComponentType.h"

namespace Engine
{
    typedef std::uint32_t Entity;
    typedef std::bitset<MAX_COMPONENTS> Signature;
    const Entity MAX_ENTITIES = 10000;
    const Entity INVALID_ENTITY_ID = MAX_ENTITIES;
} // Engine

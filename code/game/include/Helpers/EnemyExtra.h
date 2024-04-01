#pragma once
#include "KindredSpiritExtra.h"
#include "AssiExtra.h"
#include "CuballExtra.h"

union EnemyExtra
{
    KindredSpiritExtra kindredSpirit{};
    AssiExtra assi;
    CuballExtra cuball;
};

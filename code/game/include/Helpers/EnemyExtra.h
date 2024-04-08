#pragma once
#include "KindredSpiritExtra.h"
#include "AssiExtra.h"
#include "CuballExtra.h"
#include "Helpers/DukeExtra.h"

union EnemyExtra
{
    KindredSpiritExtra kindredSpirit{};
    AssiExtra assi;
    CuballExtra cuball;
    DukeExtra dukeExtra;
};

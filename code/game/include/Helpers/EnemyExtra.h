#pragma once
#include "AssiExtra.h"
#include "CuballExtra.h"
#include "Helpers/DukeExtra.h"

union EnemyExtra
{
    AssiExtra assi;
    CuballExtra cuball;
    DukeExtra dukeExtra{};
};

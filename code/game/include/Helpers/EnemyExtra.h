#pragma once
#include "CuballExtra.h"
#include "Helpers/DukeExtra.h"

union EnemyExtra
{
    CuballExtra cuball;
    DukeExtra dukeExtra{};
};

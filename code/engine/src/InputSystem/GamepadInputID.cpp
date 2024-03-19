#include "InputSystem/GamepadInputID.h"

bool Engine::GamepadInputID::operator<(Engine::GamepadInputID const &other) const
{
    if(joystickID != other.joystickID)
        return joystickID < other.joystickID;
    if(inputID != other.inputID)
        return inputID < other.inputID;
    return false;
}

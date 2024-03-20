#include "InputSystem/GamepadInputID.h"

bool Engine::GamepadInputID::operator<(Engine::GamepadInputID const &other) const
{
    if(inputType != other.inputType)
        return inputType < other.inputType;
    if(joystickID != other.joystickID)
        return joystickID < other.joystickID;
    if(inputID != other.inputID)
        return inputID < other.inputID;
    return false;
}

bool Engine::GamepadInputID::operator==(const Engine::GamepadInputID &other) const
{
    return joystickID == other.joystickID && inputID == other.inputID && inputType == other.inputType;
}

bool Engine::GamepadInputID::operator!=(const Engine::GamepadInputID &other) const
{
    return !(*this == other);
}

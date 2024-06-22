#include "InputSystem/GamepadAxis.h"
#include "Systems.h"

bool Engine::GamepadAxis::operator<(Engine::GamepadAxis const &other) const
{
    if(joystickID != other.joystickID)
        return joystickID < other.joystickID;
    if(inputID != other.inputID)
        return inputID < other.inputID;
    return false;
}

bool Engine::GamepadAxis::operator==(const Engine::GamepadAxis &other) const
{
    return joystickID == other.joystickID && inputID == other.inputID;
}

bool Engine::GamepadAxis::operator!=(const Engine::GamepadAxis &other) const
{
    return !(*this == other);
}

Engine::GamepadAxis::GamepadAxis(unsigned char gamepadID, unsigned char inputID)
{
    this->joystickID = Systems::inputSystem->FindJoystickID(gamepadID);
    this->inputID = inputID;
}

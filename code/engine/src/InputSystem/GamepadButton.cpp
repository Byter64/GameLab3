#include "InputSystem/GamepadButton.h"
#include "Systems.h"

bool Engine::GamepadButton::operator<(Engine::GamepadButton const &other) const
{
    if(joystickID != other.joystickID)
        return joystickID < other.joystickID;
    if(inputID != other.inputID)
        return inputID < other.inputID;
    return false;
}

bool Engine::GamepadButton::operator==(const Engine::GamepadButton &other) const
{
    return joystickID == other.joystickID && inputID == other.inputID;
}

bool Engine::GamepadButton::operator!=(const Engine::GamepadButton &other) const
{
    return !(*this == other);
}

Engine::GamepadButton::GamepadButton(unsigned char gamepadID, unsigned char inputID)
{
    this->joystickID = Systems::inputSystem->FindJoystickID(gamepadID);
    this->inputID = inputID;
}

#include "GLFW/glfw3.h"
#include "InputSystem/InputActionButton.h"
#include "InputSystem/InputSystem.h"

namespace Engine
{
    inline const float InputActionButton::axisTolerance = 0.2f;

    void InputActionButton::AddKeyboardBinding(int key)
    {
        keyboardBindings.push_back(key);
    }

    void InputActionButton::AddGamepadBinding(GamepadInputID inputID)
    {
        gamepadBindings.push_back(inputID);
    }

    void InputActionButton::RemoveKeyboardBinding(int key)
    {
        keyboardBindings.remove(key);
    }

    void InputActionButton::RemoveGamepadBinding(GamepadInputID inputID)
    {
        gamepadBindings.remove(inputID);
    }

    void InputActionButton::AddOnStart(void* object,CallbackButton callback)
    {
        startCallbacks.push_back(std::pair(object, callback));
    }

    void InputActionButton::RemoveOnStart(void* object,CallbackButton callback)
    {
        startCallbacks.remove(std::pair(object, callback));
    }

    void InputActionButton::AddOnEnd(void* object,CallbackButton callback)
    {
        endCallbacks.push_back(std::pair(object, callback));
    }

    void InputActionButton::RemoveOnEnd(void* object,CallbackButton callback)
    {
        endCallbacks.remove(std::pair(object, callback));
    }

    void InputActionButton::Update(int key)
    {
        bool state = inputSystem->GetKeyState(key);
        Update(state);
    }

    void InputActionButton::Update(GamepadInputID &input)
    {
        bool state;
        if(input.inputType == GamepadInputID::Button)
            state = inputSystem->GetButtonState(input.joystickID, input.inputID);
        else if(input.inputType == GamepadInputID::Axis)
            state = inputSystem->GetAxisState(input.joystickID, input.inputID) < axisTolerance;
        Update(state);
    }

    void InputActionButton::Update(bool value)
    {
        if(value == GLFW_PRESS)
        {
            for(auto pair : startCallbacks)
            {
                (*pair.second)(pair.first);
            }
        }
        else if (value == GLFW_RELEASE)
        {
            for(auto pair : endCallbacks)
            {
                (*pair.second)(pair.first);
            }
        }
    }

    InputActionButton::InputActionButton(std::string name) : InputAction(name)
    {

    }
} // Engine
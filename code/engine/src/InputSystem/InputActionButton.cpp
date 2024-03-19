#include "GLFW/glfw3.h"
#include "InputSystem/InputActionButton.h"
#include "InputSystem/InputSystem.h"

namespace Engine
{
    void InputActionButton::AddKeyboardBinding(int key)
    {
        keyboardBindings.push_back(key);
    }

    void InputActionButton::RemoveKeyboardBinding(int key)
    {
        keyboardBindings.remove(key);
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
        int action = inputSystem->GetKeyState(key);
        if(action == GLFW_PRESS)
        {
            for(auto pair : startCallbacks)
            {
                (*pair.second)(pair.first);
            }
        }
        else if (action == GLFW_RELEASE)
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

    void InputActionButton::Update(GamepadInputID &input)
    {

    }
} // Engine
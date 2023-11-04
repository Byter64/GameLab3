#include "InputActionButton.h"
#include "GLFW/glfw3.h"

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

    void InputActionButton::AddOnStart(CallbackButton callback)
    {
        startCallbacks.push_back(callback);
    }

    void InputActionButton::RemoveOnStart(CallbackButton callback)
    {
        startCallbacks.remove(callback);
    }

    void InputActionButton::AddOnEnd(CallbackButton callback)
    {
        endCallbacks.push_back(callback);
    }

    void InputActionButton::RemoveOnEnd(CallbackButton callback)
    {
        endCallbacks.remove(callback);
    }

    void InputActionButton::UpdateAction(int key)
    {
        int action = inputSystem->GetKeyState(key);
        if(action == GLFW_PRESS)
        {
            for(CallbackButton callback : startCallbacks)
            {
                (*callback)();
            }
        }
        else
        {
            for(CallbackButton callback : endCallbacks)
            {
                (*callback)();
            }
        }
    }
} // Engine
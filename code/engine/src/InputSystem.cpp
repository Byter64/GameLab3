#include "InputSystem.h"
#include "GLFW/glfw3.h"

namespace Engine
{
    InputSystem::InputSystem(GLFWwindow* window) : window(window)
    {
        glfwSetKeyCallback(window, KeyCallback);
        activeInputSystems.push_back(this);

        for(int i = 0; i < GLFW_KEY_LAST - 2; i++)
        {
            keyStates[i] = GLFW_RELEASE;
        }
    }

    InputSystem::~InputSystem()
    {
        glfwSetKeyCallback(window, nullptr);
        activeInputSystems.remove(this);
    }

    void InputSystem::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        for(InputSystem* inputSystem : activeInputSystems)
        {
            inputSystem->keyStates[key] = action;
        }
    }

    void InputSystem::Add(std::unique_ptr<InputAction> inputAction)
    {
        inputAction->inputSystem = this;
        inputActions.push_back(std::move(inputAction));
    }

    std::unique_ptr<InputAction> InputSystem::Remove(std::string inputActionName)
    {
        auto result = std::find_if(inputActions.begin(), inputActions.end(), [&inputActionName]( const std::unique_ptr<InputAction>& inputAction) {return inputAction->name ==inputActionName;});
        if(result == inputActions.end())
        {
            return nullptr;
        }
        return std::move(*result);
    }

    int InputSystem::GetKeyState(int key)
    {
        return keyStates[key];
    }


} // Engine
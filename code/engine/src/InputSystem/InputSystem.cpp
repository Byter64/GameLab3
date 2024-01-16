#include "InputSystem/InputSystem.h"
#include "GLFW/glfw3.h"

namespace Engine
{
    InputSystem::InputSystem(GLFWwindow* window) : window(window)
    {
        glfwSetKeyCallback(window, KeyCallback);
        instance = this;

        for(int i = 0; i < GLFW_KEY_LAST - 2; i++)
        {
            keyStates[i] = GLFW_RELEASE;
        }
    }

    InputSystem::~InputSystem()
    {
        glfwSetKeyCallback(window, nullptr);
    }

    void InputSystem::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        instance->keyStates[key] = action;

        for (const auto &inputAction: instance->keyToInputActions[key])
        {
            inputAction->Update(key);
        }
    }

    void InputSystem::Add(std::shared_ptr<InputAction> inputAction)
    {
        inputAction->inputSystem = this;

        for (int key: inputAction->keyboardBindings)
        {
            keyToInputActions[key].push_back(inputAction.get());
        }
        inputActions.push_back(std::move(inputAction));
    }

    std::shared_ptr<InputAction> InputSystem::Remove(std::shared_ptr<InputAction> inputAction)
    {
        auto result = std::find(inputActions.begin(), inputActions.end(), inputAction);
        if(result == inputActions.end())
        {
            return nullptr;
        }

        //Delete all references to this InputAction
        for(auto& pair : keyToInputActions)
        {
            std::list<InputAction*>& list = pair.second;
            list.remove(result->get());
        }

        //return it
        return std::move(*result);
    }

    std::shared_ptr<InputAction> InputSystem::Remove(std::string inputActionName)
    {
        //Find InputAction
        auto result = std::find_if(inputActions.begin(), inputActions.end(), [&inputActionName](const std::shared_ptr<InputAction>& inputAction) {return inputAction->name == inputActionName;});
        if(result == inputActions.end())
        {
            return nullptr;
        }

        return Remove(*result);
    }

    int InputSystem::GetKeyState(int key)
    {
        return keyStates[key];
    }

} // Engine
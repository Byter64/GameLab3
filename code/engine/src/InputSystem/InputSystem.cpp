#include "InputSystem/InputSystem.h"
#include "GLFW/glfw3.h"
#include <iostream>
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

    ///
    /// \param maxJoysticks how many joystick inputs should be checked at max
    void InputSystem::Update(unsigned char maxJoysticks)
    {
        maxJoysticks = maxJoysticks > GLFW_JOYSTICK_LAST ? GLFW_JOYSTICK_LAST : maxJoysticks;

        for(unsigned char i = 0; i < 1; i++)
        {
            GLFWgamepadstate state;
            glfwGetGamepadState(i, &state);

            for(unsigned char axis = 0; axis <= GLFW_GAMEPAD_AXIS_LAST; axis++)
            {
                GamepadInputID input = {i, axis, GamepadInputID::Axis};
                for(const auto &inputAction : instance->inputToInputActions[input])
                    if(state.axes[axis] != gamePadStates[i].axes[axis])
                        inputAction->Update(input);
            }

            for(unsigned char button = 0; button <= GLFW_GAMEPAD_BUTTON_LAST; button++)
            {
                GamepadInputID input = {i, button, GamepadInputID::Button};
                for(const auto &inputAction : instance->inputToInputActions[input])
                    if(state.buttons[button] != gamePadStates[i].buttons[button])
                        inputAction->Update(input);
            }

            gamePadStates[i] = state;
        }
    }

    void InputSystem::Add(std::shared_ptr<InputAction> inputAction)
    {
        inputAction->inputSystem = this;

        for (int key: inputAction->keyboardBindings)
            keyToInputActions[key].push_back(inputAction.get());

        for (GamepadInputID& input: inputAction->gamepadBindings)
            inputToInputActions[input].push_back(inputAction.get());

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

        result = inputActions.erase(result);
        //return it
        return *result;
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
#include "InputSystem/InputActionVec2.h"
#include "glm/glm.hpp"
#include "InputSystem/InputSystem.h"
#include <algorithm>
#include <iostream>

namespace Engine
{
    InputActionVec2::InputActionVec2(std::string name) : InputAction(name)
    {
        lastFloatInput = glm::vec2(0, 0);
    }

    void InputActionVec2::AddKeyboardBinding(int leftKey, int rightKey, int upKey, int downKey)
    {
        DirectionMap map(leftKey, rightKey, upKey, downKey);
        keyDirectionMaps.push_back(map);

        keyboardBindings.push_back(leftKey);
        keyboardBindings.push_back(rightKey);
        keyboardBindings.push_back(upKey);
        keyboardBindings.push_back(downKey);
    }

    void InputActionVec2::AddGamepadAxesBinding(GamepadAxis xAxis, GamepadAxis yAxis)
    {
        axes.push_back({xAxis,yAxis});

        gamepadAxes.push_back(xAxis);
        gamepadAxes.push_back(yAxis);
    }

    void InputActionVec2::AddGamepadButtonBinding(GamepadButton leftButton, GamepadButton rightButton, GamepadButton upButton, GamepadButton downButton)
    {
        gamepadDirectionMaps.push_back({leftButton, rightButton, upButton, downButton});

        gamepadButtons.push_back(leftButton);
        gamepadButtons.push_back(rightButton);
        gamepadButtons.push_back(upButton);
        gamepadButtons.push_back(downButton);
    }

    void InputActionVec2::RemoveKeyboardBinding(int leftKey, int rightKey, int upKey, int downKey)
    {
        DirectionMap map(leftKey, rightKey, upKey, downKey);

        if(std::find(keyDirectionMaps.begin(), keyDirectionMaps.end(), map) == keyDirectionMaps.end())
            return;

        keyDirectionMaps.remove(map);

        keyboardBindings.remove(leftKey);
        keyboardBindings.remove(rightKey);
        keyboardBindings.remove(upKey);
        keyboardBindings.remove(downKey);
    }

    void InputActionVec2::RemoveGamepadAxesBinding(GamepadAxis xAxis, GamepadAxis yAxis)
    {
        std::pair<GamepadAxis, GamepadAxis> axes = {xAxis, yAxis};

        if(std::find(this->axes.begin(), this->axes.end(), axes) == this->axes.end())
            return;

        this->axes.remove(axes);

        gamepadAxes.remove(xAxis);
        gamepadAxes.remove(yAxis);
    }

    void InputActionVec2::RemoveGamepadButtonBinding(GamepadButton leftButton, GamepadButton rightButton, GamepadButton upButton, GamepadButton downButton)
    {
        DirectionMap map(leftButton, rightButton, upButton, downButton);
        if(std::find(gamepadDirectionMaps.begin(), gamepadDirectionMaps.end(), map) == gamepadDirectionMaps.end())
            return;

        this->gamepadDirectionMaps.remove(map);

        gamepadButtons.remove(leftButton);
        gamepadButtons.remove(rightButton);
        gamepadButtons.remove(upButton);
        gamepadButtons.remove(downButton);
    }

    void InputActionVec2::AddOnStart(void* object, InputActionVec2::CallbackVec2 callback)
    {
        startCallbacks.push_back(std::make_pair(object, callback));
    }

    void InputActionVec2::RemoveOnStart(void* object, InputActionVec2::CallbackVec2 callback)
    {
        startCallbacks.remove(std::make_pair(object, callback));
    }

    void InputActionVec2::AddOnValueChange(void* object, InputActionVec2::CallbackVec2 callback)
    {
        valueChangeCallbacks.push_back(std::make_pair(object, callback));
    }

    void InputActionVec2::RemoveOnValueChange(void* object, InputActionVec2::CallbackVec2 callback)
    {
        valueChangeCallbacks.remove(std::make_pair(object, callback));
    }

    void InputActionVec2::AddOnEnd(void* object, InputActionVec2::CallbackVec2 callback)
    {
        endCallbacks.push_back(std::make_pair(object, callback));
    }

    void InputActionVec2::RemoveOnEnd(void* object, InputActionVec2::CallbackVec2 callback)
    {
        endCallbacks.remove(std::make_pair(object, callback));
    }

    void InputActionVec2::Update(int key)
    {
        auto result = std::find_if(keyDirectionMaps.begin(), keyDirectionMaps.end(), [&key](const DirectionMap<int>& map){return key == map.leftKey || key == map.rightKey || key == map.upKey || key == map.downKey;});
        const DirectionMap<int>& map = *result;

        int x = 0,y = 0;
        x -= std::clamp(inputSystem->GetKeyState(map.leftKey), 0, 1);
        x += std::clamp(inputSystem->GetKeyState(map.rightKey), 0, 1);

        y -= std::clamp(inputSystem->GetKeyState(map.downKey), 0, 1);
        y += std::clamp(inputSystem->GetKeyState(map.upKey), 0, 1);

        glm::vec2 input(x, y);

        if(input != glm::vec2(0))
            input = glm::normalize(input);
        UpdateInt(input);
    }

    void InputActionVec2::Update(GamepadButton &button)
    {
        auto result = std::find_if(gamepadDirectionMaps.begin(), gamepadDirectionMaps.end(), [&button](const DirectionMap<GamepadButton>& map){return button == map.leftKey || button == map.rightKey || button == map.upKey || button == map.downKey;});
        const DirectionMap<GamepadButton>& map = *result;

        int x = 0,y = 0;
        x -= std::clamp(inputSystem->GetButtonState(map.leftKey.joystickID, map.leftKey.inputID), 0, 1);
        x += std::clamp(inputSystem->GetButtonState(map.rightKey.joystickID, map.rightKey.inputID), 0, 1);

        y -= std::clamp(inputSystem->GetButtonState(map.downKey.joystickID, map.downKey.inputID), 0, 1);
        y += std::clamp(inputSystem->GetButtonState(map.upKey.joystickID, map.upKey.inputID), 0, 1);

        glm::vec2 input(x, y);

        if(input != glm::vec2(0))
            input = glm::normalize(input);
        UpdateInt(input);
    }

    void InputActionVec2::Update(GamepadAxis& input)
    {
        auto result = std::find_if(axes.begin(), axes.end(), [&input](const std::pair<GamepadAxis, GamepadAxis>& axesPair)
        {
            bool isXAxis = axesPair.first == input;
            bool isYAxis = axesPair.second == input;
            return isXAxis || isYAxis;
        });

        const std::pair<GamepadAxis, GamepadAxis>& axes = *result;

        float x = inputSystem->GetAxisState(axes.first.joystickID, axes.first.inputID);
        float y = inputSystem->GetAxisState(axes.second.joystickID, axes.second.inputID);
        glm::vec2 value(x, y);
        UpdateFloat(value);
    }

    void InputActionVec2::Update()
    {
        glm::vec2 direction = actualFloatInput - lastFloatInput;
        direction.x = direction.x / ::abs(direction.x);
        direction.y = direction.y / ::abs(direction.y);

        glm::vec2 newInput = lastFloatInput + direction * maxChange;
        if (std::signbit(newInput.x - actualFloatInput.x) != std::signbit(lastFloatInput.x - actualFloatInput.x))
            newInput.x = actualFloatInput.x;
        if (std::signbit(newInput.y - actualFloatInput.y) != std::signbit(lastFloatInput.y - actualFloatInput.y))
            newInput.y = actualFloatInput.y;

        glm::vec2 oldDeadZoned, newDeadZoned;
        oldDeadZoned.x = glm::abs(lastInput.x) < deadzone ? 0.0f : lastInput.x;
        oldDeadZoned.y = glm::abs(lastInput.y) < deadzone ? 0.0f : lastInput.y;
        newDeadZoned.x = glm::abs(newInput.x) < deadzone ? 0.0f : newInput.x;
        newDeadZoned.y = glm::abs(newInput.y) < deadzone ? 0.0f : newInput.y;
        if (oldDeadZoned == newDeadZoned)
        {
            lastFloatInput = newInput;
            lastInput = newInput;
            return;
        }
        //prioritise int inputs (i.e. d-pad) over joystick
        if(lastIntInput != glm::vec2(0))
            return;

        //Check for end of input
        if(newDeadZoned == glm::vec2(0,0) && oldDeadZoned != glm::vec2 (0,0))
        {
            for (auto pair: valueChangeCallbacks)
            {
                pair.second(pair.first, newDeadZoned);
            }
            for (auto pair: endCallbacks)
            {
                pair.second(pair.first, newDeadZoned);
            }
            lastFloatInput = newInput;
            lastInput = newInput;
            return;
        }

        //Check for start of input
        if(oldDeadZoned == glm::vec2(0,0))
        {
            for (auto pair: startCallbacks)
            {
                pair.second(pair.first, newDeadZoned);
            }
            for (auto pair: valueChangeCallbacks)
            {
                pair.second(pair.first, newDeadZoned);
            }
            lastFloatInput = newInput;
            lastInput = newInput;
            return;
        }

            //Check for value change of input
        else if(newDeadZoned != oldDeadZoned)
        {
            for (auto pair: valueChangeCallbacks)
            {
                pair.second(pair.first, newDeadZoned);
            }
            lastFloatInput = newInput;
            lastInput = newInput;
            return;
        }
    }

    void InputActionVec2::UpdateFloat(glm::vec2 value)
    {
        //Events for floats are fired in regular Update
        actualFloatInput = value;
    }

    void InputActionVec2::UpdateInt(glm::vec2 value)
    {
        //Check for end of input
        if(value == glm::vec2(0,0) && lastInput != glm::vec2 (0,0))
        {
            for (auto pair: valueChangeCallbacks)
            {
                pair.second(pair.first, value);
            }
            for (auto pair: endCallbacks)
            {
                pair.second(pair.first, value);
            }
            lastIntInput = value;
            lastInput = value;
            return;
        }

        //Check for start of input
        if(lastInput == glm::vec2(0,0))
        {
            for (auto pair: startCallbacks)
            {
                pair.second(pair.first, value);
            }
            for (auto pair: valueChangeCallbacks)
            {
                pair.second(pair.first, value);
            }
            lastIntInput = value;
            lastInput = value;
            return;
        }

            //Check for value change of input
        else if(value != lastInput)
        {
            for (auto pair: valueChangeCallbacks)
            {
                pair.second(pair.first, value);
            }
            lastIntInput = value;
            lastInput = value;
            return;
        }
    }
}

#include "InputSystem/InputActionVec2.h"
#include "glm/glm.hpp"
#include "InputSystem/InputSystem.h"
#include <algorithm>
#include <iostream>

namespace Engine
{
    InputActionVec2::DirectionMap::DirectionMap()
    {
        leftKey = -1;
        rightKey = -1;
        upKey = -1;
        downKey = -1;
    }

    InputActionVec2::DirectionMap::DirectionMap(int leftKey, int rightKey, int upKey, int downKey) : leftKey(leftKey), rightKey(rightKey), upKey(upKey), downKey(downKey)
    {

    }

    bool InputActionVec2::DirectionMap::operator==(const InputActionVec2::DirectionMap &map) const
    {
        return leftKey == map.leftKey &&
        rightKey == map.rightKey &&
        upKey == map.upKey &&
        downKey == map.downKey;
    }

    bool InputActionVec2::DirectionMap::operator!=(const InputActionVec2::DirectionMap &map) const
    {
        return !((*this) == map);
    }

    InputActionVec2::InputActionVec2(std::string name) : InputAction(name)
    {
        oldInput = glm::vec2(0,0);
    }

    void InputActionVec2::AddKeyboardBinding(int leftKey, int rightKey, int upKey, int downKey)
    {
        DirectionMap map(leftKey, rightKey, upKey, downKey);
        directionMaps.push_back(map);

        keyboardBindings.push_back(leftKey);
        keyboardBindings.push_back(rightKey);
        keyboardBindings.push_back(upKey);
        keyboardBindings.push_back(downKey);
    }

    void InputActionVec2::AddGamepadAxesBinding(GamepadInputID &xAxis, GamepadInputID &yAxis)
    {
        axes.push_back({{GamepadInputID::Axis, xAxis}, {GamepadInputID::Axis, yAxis}});

        gamepadBindings.push_back(xAxis);
        gamepadBindings.push_back(yAxis);
    }

    void InputActionVec2::AddGamepadButtonBinding(GamepadInputID &leftButton, GamepadInputID &rightButton,GamepadInputID &upButton, GamepadInputID &downButton)
    {
        Axis xAxis, yAxis;
        xAxis.type = GamepadInputID::Button;
        xAxis.buttons.b1 = leftButton;
        xAxis.buttons.b2 = rightButton;

        yAxis.type = GamepadInputID::Button;
        yAxis.buttons.b1 = upButton;
        yAxis.buttons.b2 = downButton;

        axes.push_back({xAxis, yAxis});

        gamepadBindings.push_back(leftButton);
        gamepadBindings.push_back(rightButton);
        gamepadBindings.push_back(upButton);
        gamepadBindings.push_back(downButton);
    }

    void InputActionVec2::RemoveKeyboardBinding(int leftKey, int rightKey, int upKey, int downKey)
    {
        DirectionMap map(leftKey, rightKey, upKey, downKey);

        if(std::find(directionMaps.begin(), directionMaps.end(), map) == directionMaps.end())
            return;

        directionMaps.remove(map);

        keyboardBindings.remove(leftKey);
        keyboardBindings.remove(rightKey);
        keyboardBindings.remove(upKey);
        keyboardBindings.remove(downKey);
    }

    void InputActionVec2::RemoveGamepadAxesBinding(GamepadInputID &xAxis, GamepadInputID &yAxis)
    {
        std::pair<Axis, Axis> axes = {{GamepadInputID::Axis, xAxis}, {GamepadInputID::Axis, yAxis}};

        if(std::find(this->axes.begin(), this->axes.end(), axes) == this->axes.end())
            return;

        this->axes.remove(axes);

        gamepadBindings.remove(xAxis);
        gamepadBindings.remove(yAxis);
    }

    void InputActionVec2::RemoveGamepadButtonBinding(GamepadInputID &leftButton, GamepadInputID &rightButton, GamepadInputID &upButton, GamepadInputID &downButton)
    {
        Axis xAxis, yAxis;
        xAxis.type = GamepadInputID::Button;
        xAxis.buttons.b1 = leftButton;
        xAxis.buttons.b2 = rightButton;

        yAxis.type = GamepadInputID::Button;
        yAxis.buttons.b1 = upButton;
        yAxis.buttons.b2 = downButton;

        std::pair<Axis, Axis> axes = {xAxis, yAxis};

        if(std::find(this->axes.begin(), this->axes.end(), axes) == this->axes.end())
            return;

        this->axes.remove(axes);

        gamepadBindings.remove(leftButton);
        gamepadBindings.remove(rightButton);
        gamepadBindings.remove(upButton);
        gamepadBindings.remove(downButton);
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
        auto result = std::find_if(directionMaps.begin(), directionMaps.end(), [&key](const DirectionMap& map){return key == map.leftKey || key == map.rightKey || key == map.upKey || key == map.downKey;});
        const DirectionMap& map = *result;

        int x = 0,y = 0;
        x -= std::clamp(inputSystem->GetKeyState(map.leftKey), 0, 1);
        x += std::clamp(inputSystem->GetKeyState(map.rightKey), 0, 1);

        y -= std::clamp(inputSystem->GetKeyState(map.downKey), 0, 1);
        y += std::clamp(inputSystem->GetKeyState(map.upKey), 0, 1);

        glm::vec2 input(x, y);

        if(input != glm::vec2(0))
            input = glm::normalize(input);
        Update(input);
    }

    void InputActionVec2::Update(GamepadInputID &input)
    {
        auto result = std::find_if(axes.begin(), axes.end(), [&input](const std::pair<Axis, Axis>& axesPair)
        {
            bool isXAxis = false, isYAxis = false;
            if(axesPair.first.type == GamepadInputID::Axis)
                isXAxis = axesPair.first.axis == input;
            else if(axesPair.first.type == GamepadInputID::Button)
                isXAxis = axesPair.first.buttons.b1 == input || axesPair.first.buttons.b2 == input;

            if(axesPair.second.type == GamepadInputID::Axis)
                isYAxis = axesPair.second.axis == input;
            else if(axesPair.second.type == GamepadInputID::Button)
                isYAxis = axesPair.second.buttons.b1 == input || axesPair.second.buttons.b2 == input;

            return isXAxis || isYAxis;
        });

        const std::pair<Axis, Axis>& axes = *result;

        float x = GetValue(axes.first);
        float y = GetValue(axes.second);

        glm::vec2 value(x, y);
        Update(value);
    }

    void InputActionVec2::Update(glm::vec2 value)
    {
        //Check for end of input
        if(value == glm::vec2(0,0) && oldInput != glm::vec2 (0,0))
        {
            for (auto pair: valueChangeCallbacks)
            {
                pair.second(pair.first, value);
            }
            for (auto pair: endCallbacks)
            {
                pair.second(pair.first, value);
            }
            oldInput = value;
            return;
        }

        //Check for start of input
        if(oldInput == glm::vec2(0,0))
        {
            for (auto pair: startCallbacks)
            {
                pair.second(pair.first, value);
            }
            for (auto pair: valueChangeCallbacks)
            {
                pair.second(pair.first, value);
            }
            oldInput = value;
            return;
        }

        //Check for value change of input
        else if(value != oldInput)
        {
            for (auto pair: valueChangeCallbacks)
            {
                pair.second(pair.first, value);
            }
            oldInput = value;
            return;
        }
    }

    float InputActionVec2::GetValue(InputActionVec2::Axis const &axis)
    {
        if(axis.type == GamepadInputID::Axis)
            return inputSystem->GetAxisState(axis.axis.joystickID, axis.axis.inputID);
        else if(axis.type == GamepadInputID::Button)
            return (float)(-inputSystem->GetButtonState(axis.buttons.b1.joystickID, axis.buttons.b1.inputID) + inputSystem->GetButtonState(axis.buttons.b2.joystickID, axis.buttons.b2.inputID));
        return 0.0f;
    }

    bool InputActionVec2::Axis::operator==(const InputActionVec2::Axis &other) const
    {
        if(type == GamepadInputID::Axis)
            return type == other.type && axis == other.axis;
        else if(type == GamepadInputID::Button)
            return type == other.type && buttons.b1 == other.buttons.b1 && buttons.b2 == other.buttons.b2;
        return false;
    }

    bool InputActionVec2::Axis::operator!=(const InputActionVec2::Axis &other) const
    {
        return !(*this == other);
    }
}

#include "InputSystem/InputActionVec2.h"
#include "glm/glm.hpp"
#include "InputSystem/InputSystem.h"
#include <algorithm>

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

    void InputActionVec2::RemoveKeyboardBinding(int leftKey, int rightKey, int upKey, int downKey)
    {
        DirectionMap map(leftKey, rightKey, upKey, downKey);

        if(std::find(directionMaps.begin(), directionMaps.end(), map) == directionMaps.end())
        {
            return;
        }

        directionMaps.remove(map);

        keyboardBindings.remove(leftKey);
        keyboardBindings.remove(rightKey);
        keyboardBindings.remove(upKey);
        keyboardBindings.remove(downKey);
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

        //Check for end of input
        glm::vec2 input(x,y);
        if(input == glm::vec2(0,0) && oldInput != glm::vec2 (0,0))
        {
            for (auto pair: valueChangeCallbacks)
            {
                pair.second(pair.first, input);
            }
            for (auto pair: endCallbacks)
            {
                pair.second(pair.first, input);
            }
            oldInput = input;
            return;
        }


        //Check for start of input
        input = glm::normalize(input);
        if(oldInput == glm::vec2(0,0))
        {
            for (auto pair: valueChangeCallbacks)
            {
                pair.second(pair.first, input);
            }
            for (auto pair: startCallbacks)
            {
                pair.second(pair.first, input);
            }
            oldInput = input;
            return;
        }
        //Check for value change of input
        else if(input != oldInput)
        {
            for (auto pair: valueChangeCallbacks)
            {
                pair.second(pair.first, input);
            }
            oldInput = input;
            return;
        }
    }
}

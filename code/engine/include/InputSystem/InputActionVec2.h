#pragma once
#include "InputAction.h"
#include "glm/vec2.hpp"
#include "TimeManager.h"
#include "InputSystem/GamepadAxis.h"
#include "InputSystem/GamepadButton.h"

namespace Engine
{

    class InputActionVec2 : public InputAction
    {
        template<typename T>
        struct DirectionMap
        {
            T leftKey;
            T rightKey;
            T upKey;
            T downKey;

            DirectionMap(T leftKey, T rightKey, T upKey, T downKey) : leftKey(leftKey), rightKey(rightKey), upKey(upKey), downKey(downKey)
            { }

            bool operator==(const DirectionMap &map) const
            {
                return leftKey == map.leftKey &&
                       rightKey == map.rightKey &&
                       upKey == map.upKey &&
                       downKey == map.downKey;
            }

            bool operator!=(const DirectionMap &map) const
            {
                return !((*this) == map);
            }
        };

        inline static const float deadzone = 0.2f;
        inline static const float maxChange = 20.0f * (FRAMETIME / 1000000.0f);
        typedef void (*CallbackVec2)(void*, glm::vec2 input);

        glm::vec2 lastFloatInput{0};
        glm::vec2 lastIntInput{0};
        glm::vec2 lastInput{0};
        glm::vec2 actualFloatInput{0};
        std::list<DirectionMap<int>> keyDirectionMaps;
        std::list<DirectionMap<GamepadButton>> gamepadDirectionMaps;
        std::list<std::pair<GamepadAxis, GamepadAxis>> axes;
        std::list<std::pair<void*, CallbackVec2>> startCallbacks;
        std::list<std::pair<void*, CallbackVec2>> valueChangeCallbacks;
        std::list<std::pair<void*, CallbackVec2>> endCallbacks;

        void UpdateFloat(glm::vec2 value);
        void UpdateInt(glm::vec2 value);
    public:
        explicit InputActionVec2(std::string name);

        void AddKeyboardBinding(int leftKey, int rightKey, int upKey, int downKey);
        void AddGamepadAxesBinding(GamepadAxis xAxis, GamepadAxis yAxis);
        void AddGamepadButtonBinding(GamepadButton leftButton, GamepadButton rightButton, GamepadButton upButton, GamepadButton downButton);
        void RemoveKeyboardBinding(int leftKey, int rightKey, int upKey, int downKey);
        void RemoveGamepadAxesBinding(GamepadAxis xAxis, GamepadAxis yAxis);
        void RemoveGamepadButtonBinding(GamepadButton leftButton, GamepadButton rightButton, GamepadButton upButton, GamepadButton downButton);

        void AddOnStart(void* object, CallbackVec2 callback);
        void RemoveOnStart(void* object, CallbackVec2 callback);

        void AddOnValueChange(void* object, CallbackVec2 callback);
        void RemoveOnValueChange(void* object, CallbackVec2 callback);

        void AddOnEnd(void* object, CallbackVec2 callback);
        void RemoveOnEnd(void* object, CallbackVec2 callback);

        void Update(int key) override;
        void Update(GamepadButton& button) override;
        void Update(GamepadAxis& input) override;
        void Update() override;
    };
} // Engine

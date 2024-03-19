#pragma once
#include "InputAction.h"
#include "glm/vec2.hpp"

namespace Engine
{

    class InputActionVec2 : public InputAction
    {
        typedef void (*CallbackVec2)(void*, glm::vec2 input);
        struct DirectionMap;

        glm::vec2 oldInput;
        std::list<DirectionMap> directionMaps;
        std::list<std::pair<void*, CallbackVec2>> startCallbacks;
        std::list<std::pair<void*, CallbackVec2>> valueChangeCallbacks;
        std::list<std::pair<void*, CallbackVec2>> endCallbacks;
    public:
        explicit InputActionVec2(std::string name);

        void AddKeyboardBinding(int leftKey, int rightKey, int upKey, int downKey);
        void RemoveKeyboardBinding(int leftKey, int rightKey, int upKey, int downKey);

        void AddOnStart(void* object, CallbackVec2 callback);
        void RemoveOnStart(void* object, CallbackVec2 callback);

        void AddOnValueChange(void* object, CallbackVec2 callback);
        void RemoveOnValueChange(void* object, CallbackVec2 callback);

        void AddOnEnd(void* object, CallbackVec2 callback);
        void RemoveOnEnd(void* object, CallbackVec2 callback);

        void Update(int key) override;
        void Update(GamepadInputID& input) override;

    private:
        struct DirectionMap
        {
            int leftKey;
            int rightKey;
            int upKey;
            int downKey;

            DirectionMap();
            DirectionMap(int leftKey, int rightKey, int upKey, int downKey);

            bool operator==(const DirectionMap& map) const;
            bool operator!=(const DirectionMap& map) const;
        };
    };
} // Engine

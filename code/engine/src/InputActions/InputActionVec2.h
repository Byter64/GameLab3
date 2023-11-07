#pragma once
#include "InputAction.h"
#include "glm/vec2.hpp"

namespace Engine
{

    class InputActionVec2 : public InputAction
    {
        typedef void (*CallbackVec2)(glm::vec2 input);
        struct DirectionMap;

        glm::vec2 oldInput;
        std::list<DirectionMap> directionMaps;
        std::list<CallbackVec2> startCallbacks;
        std::list<CallbackVec2> valueChangeCallbacks;
        std::list<CallbackVec2> endCallbacks;
    public:
        explicit InputActionVec2(std::string name);

        void AddKeyboardBinding(int leftKey, int rightKey, int upKey, int downKey);
        void RemoveKeyboardBinding(int leftKey, int rightKey, int upKey, int downKey);

        void AddOnStart(CallbackVec2 callback);
        void RemoveOnStart(CallbackVec2 callback);

        void AddOnValueChange(CallbackVec2 callback);
        void RemoveOnValueChange(CallbackVec2 callback);

        void AddOnEnd(CallbackVec2 callback);
        void RemoveOnEnd(CallbackVec2 callback);

        void Update(int key) override;

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

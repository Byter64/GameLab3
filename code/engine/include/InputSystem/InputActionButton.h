#pragma once
#include "InputAction.h"
#include <list>

namespace Engine
{
    typedef void (*CallbackButton)(void*);

    class InputActionButton : public InputAction
    {
        static const float axisTolerance;
        std::list<std::pair<void*, CallbackButton>> startCallbacks;
        std::list<std::pair<void*, CallbackButton>> endCallbacks;
        void Update(bool value);
    public:
        explicit InputActionButton(std::string name);

        void AddKeyboardBinding(int key);
        void AddGamepadBinding(GamepadButton inputID);
        void RemoveKeyboardBinding(int key);
        void RemoveGamepadBinding(GamepadButton inputID);

        void AddOnStart(void* object, CallbackButton callback);
        void RemoveOnStart(void* object, CallbackButton callback);

        void AddOnEnd(void* object, CallbackButton callback);
        void RemoveOnEnd(void* object, CallbackButton callback);

        void Update(int key) override;
        void Update(GamepadButton& input) override;
        void Update(GamepadAxis& input) override;
        void Update() override;
    };

} // Engine

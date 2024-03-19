#pragma once
#include "InputAction.h"
#include <list>

namespace Engine
{
    typedef void (*CallbackButton)(void*);

    class InputActionButton : public InputAction
    {

        std::list<std::pair<void*, CallbackButton>> startCallbacks;
        std::list<std::pair<void*, CallbackButton>> endCallbacks;
    public:
        explicit InputActionButton(std::string name);

        void AddKeyboardBinding(int key);
        void RemoveKeyboardBinding(int key);

        void AddOnStart(void* object, CallbackButton callback);
        void RemoveOnStart(void* object, CallbackButton callback);

        void AddOnEnd(void* object, CallbackButton callback);
        void RemoveOnEnd(void* object, CallbackButton callback);

        void Update(int key) override;
        void Update(GamepadInputID& input) override;
    };

} // Engine

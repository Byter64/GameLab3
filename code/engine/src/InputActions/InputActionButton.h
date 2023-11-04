#pragma once
#include "InputAction.h"
#include <list>


namespace Engine
{

    typedef void (*CallbackButton)();

    class InputActionButton : InputAction
    {
        std::list<CallbackButton> startCallbacks;
        std::list<CallbackButton> endCallbacks;
    public:
        void AddKeyboardBinding(int key);
        void RemoveKeyboardBinding(int key);

        void AddOnStart(CallbackButton callback);
        void RemoveOnStart(CallbackButton callback);

        void AddOnEnd(CallbackButton callback);
        void RemoveOnEnd(CallbackButton callback);

        void UpdateAction(int key) override;
    };

} // Engine

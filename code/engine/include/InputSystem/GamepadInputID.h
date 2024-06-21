#pragma once

namespace Engine
{

    struct GamepadInputID
    {
        enum InputType : unsigned char
        {
            Button,
            Axis
        };

        GamepadInputID() = default;

        ///
        /// \param gamepadID This is the index of the gamepad you want to reference. The first gamepad has ID 0, the second ID 1, and so on. Internally, the GLFW_JOYSTICK_... will be searched for this gamepad
        /// \param inputID
        /// \param inputType
        GamepadInputID(unsigned char gamepadID, unsigned char inputID, InputType inputType);

        unsigned char joystickID;
        unsigned char inputID; //either button ID or axis ID
        InputType inputType;

        bool operator ==(GamepadInputID const& other) const;
        bool operator !=(GamepadInputID const& other) const;
        bool operator <(GamepadInputID const& other) const;
    };

} // Engine

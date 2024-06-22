#pragma once

namespace Engine
{

    struct GamepadButton
    {
        GamepadButton() = default;

        ///
        /// \param gamepadID This is the index of the gamepad you want to reference. The first gamepad has ID 0, the second ID 1, and so on. Internally, the GLFW_JOYSTICK_... will be searched for this gamepad
        /// \param inputID
        /// \param inputType
        GamepadButton(unsigned char gamepadID, unsigned char inputID);

        unsigned char joystickID;
        unsigned char inputID; //must be a button

        bool operator ==(GamepadButton const& other) const;
        bool operator !=(GamepadButton const& other) const;
        bool operator <(GamepadButton const& other) const;
    };

} // Engine

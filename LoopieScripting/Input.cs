namespace Loopie
{
    public class Input
    {
        public static bool IsKeyDown(KeyCode keycode)
        {
            return InternalCalls.Input_IsKeyDown(keycode);
        }

        public static bool IsKeyUp(KeyCode keycode)
        {
            return InternalCalls.Input_IsKeyUp(keycode);
        }

        public static bool IsKeyPressed(KeyCode keycode)
        {
            return InternalCalls.Input_IsKeyPressed(keycode);
        }

        public static bool IsMouseButtonDown(MouseButton mouseButton)
        {
            return InternalCalls.Input_IsMouseButtonDown(mouseButton);
        }

        public static bool IsMouseButtonUp(MouseButton mouseButton)
        {
            return InternalCalls.Input_IsMouseButtonUp(mouseButton);
        }

        public static bool IsMouseButtonPressed(MouseButton mouseButton)
        {
            return InternalCalls.Input_IsMouseButtonPressed(mouseButton);
        }

        public static bool IsGamepadButtonDown(GamepadButton gamepadButton)
        {
            return InternalCalls.Input_IsGamepadButtonDown(gamepadButton);
        }

        public static bool IsGamepadButtonUp(GamepadButton gamepadButton)
        {
            return InternalCalls.Input_IsGamepadButtonUp(gamepadButton);
        }

        public static bool IsGamepadButtonPressed(GamepadButton gamepadButton)
        {
            return InternalCalls.Input_IsGamepadButtonPressed(gamepadButton);
        }
    }
}
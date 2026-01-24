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
        public static Vector2 mousePosition
        {get { return GetMousePosition(); }}
        public static Vector2 mouseDelta
        {get { return GetMouseDelta(); } }
        public static Vector2 scrollDelta
        {get { return GetScrollDelta(); } }

        private Vector2 GetMousePosition()
        {
            Vector2 mousePosition = Vector2.Zero;
            InternalCalls.Input_GetMousePosition(out mousePosition);
            return mousePosition;
        }
        private Vector2 GetMouseDelta()
        {
            Vector2 mouseDelta = Vector2.Zero;
            InternalCalls.Input_GetMousePosition(out mouseDelta);
            return mouseDelta;
        }
        private Vector2 GetScrollDelta()
        {
            Vector2 scrollDelta = Vector2.Zero;
            InternalCalls.Input_GetMousePosition(out scrollDelta);
            return scrollDelta;
        }
    }
}
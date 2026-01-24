using System;
using System.Runtime.CompilerServices;

namespace Loopie
{
    public static class InternalCalls
    {
        #region Log
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static void NativeLog(string parameter);

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static void NativeLog_Int(int parameter);

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static void NativeLog_Float(float parameter);

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static void NativeLog_Vector2(Vector2 parameter);

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static void NativeLog_Vector3(Vector3 parameter);

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static void NativeLog_Vector4(Vector4 parameter);
        #endregion
        #region Transform
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static void Transform_GetPosition(string id, out Vector3 position);

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static void Transform_SetPosition(string id, Vector3 position);
        #endregion

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsKeyDown(KeyCode key);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsKeyUp(KeyCode key);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsKeyPressed(KeyCode key);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsMouseButtonDown(MouseButton mouseButton);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsMouseButtonUp(MouseButton mouseButton);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsMouseButtonPressed(MouseButton mouseButton);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsGamepadButtonDown(GamepadButton gamepadButton);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsGamepadButtonUp(GamepadButton gamepadButton);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsGamepadButtonPressed(GamepadButton gamepadButton);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float Time_GetDeltaTime();
    }
}
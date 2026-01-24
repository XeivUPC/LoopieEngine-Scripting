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

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static void Transform_GetLocalPosition(string id, out Vector3 position);

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static void Transform_SetLocalPosition(string id, Vector3 position);

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static void Transform_GetRotation(string id, out Vector3 rotation);

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static void Transform_SetRotation(string id, Vector3 rotation);

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static void Transform_GetLocalRotation(string id, out Vector3 rotation);

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static void Transform_SetLocalRotation(string id, Vector3 rotation);

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static void Transform_GetLocalScale(string id, out Vector3 scale);

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            internal extern static void Transform_SetLocalScale(string id, Vector3 scale);
        #endregion
        #region Input
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
        #endregion
        #region Time
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float Time_GetDeltaTime();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float Time_GetFixedDeltaTime();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float Time_GetTimeScale();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Time_SetTimeScale(float scale);
        #endregion
        #region Entity
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static object Entity_GetScriptInstance(string entityID, string componentFullName);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static string Entity_Create(string entityName, string parentId);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Entity_AddComponent(string entityID, string componentFullName);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Entity_HasComponent(string entityID, Type componentType);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static string Entity_FindEntityByName(string name);
        #endregion
    }
}
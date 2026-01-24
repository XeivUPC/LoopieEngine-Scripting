using System;

namespace Loopie
{
    public class Main
    {
        public float floatTest;
        public int intTest;
        public string stringTest;

        public Main()
        {
            Console.WriteLine("Main constructor!");
            InternalCalls.NativeLog("Hola");
        }

        public void PrintMessage()
        {
            Console.WriteLine("Hello World from C#!");
        }

        public void PrintInt(int x)
        {
            Console.WriteLine($"Hello number: {x}");
        }

        public void PrintCustomMessage(string message)
        {
            Console.WriteLine($"C# says: {message}");
        }
        
        public void OnCreate()
        {
            Console.WriteLine($"Creating");
        }

        public void OnUpdate()
        {
            if (Input.IsGamepadButtonDown(GamepadButton.GAMEPAD_A))
            {
                InternalCalls.NativeLog_Float(Time.deltaTime);
                Console.WriteLine($"Updating");
            }
        }
    }
}
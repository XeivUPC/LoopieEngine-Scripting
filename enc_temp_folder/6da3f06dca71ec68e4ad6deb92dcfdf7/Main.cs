using System;
using System.Diagnostics;

namespace Loopie
{
    public class Tank : Component
    {
        public float speed;

        public Main()
        {
            Console.WriteLine("Main constructor!");
        }

        public void PrintMessage()
        {
            Console.WriteLine($"Hello World from C#!");
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
            Console.WriteLine($"Creating {entity.ID}");
            if(entity.HasComponent<Transform>())
            {
                PrintInt(0);
            }

            Main selfData = entity.GetComponent<Main>();
            if (selfData != null)
            {
                selfData.speed = 12;
                PrintInt(2);
            }
            else
            {
                PrintInt(-2);
            }
        }

        public void OnUpdate()
        {
 
            if(Input.IsKeyPressed(KeyCode.W))
            {
                entity.transform.position += transform.Back * Time.deltaTime * speed;
            }

            if (Input.IsKeyPressed(KeyCode.S))
            {
                entity.transform.position += transform.Forward * Time.deltaTime * speed;
            }

            if (Input.IsKeyPressed(KeyCode.A))
            {
                entity.transform.position += transform.Right * Time.deltaTime * speed;
            }

            if (Input.IsKeyPressed(KeyCode.D))
            {
                entity.transform.position += transform.Left * Time.deltaTime * speed;
            }

            if(Input.IsMouseButtonPressed(MouseButton.MOUSE_LEFT))
            {
                PrintCustomMessage(Convert.ToString(Time.deltaTime));
            }
        }
    }
}

using System;

namespace Loopie
{
    public class Main : Component
    {
        public float speed = 10;
        public float projectileSpeed = 20.0f;

        public Main()
        {
            Console.WriteLine("Tank constructor!");
        }
 
        public void OnCreate()
        {

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

            if (Input.IsKeyDown(KeyCode.SPACE))
            {
                Entity proj = Entity.Instantiate("Projectile");
                proj.transform.position = transform.position;
                proj.AddComponent<Projectile>();

                Projectile projComp = proj.GetComponent<Projectile>();
                projComp.direction = transform.Forward;
                projComp.speed = projectileSpeed;
            }

        }
    }
}

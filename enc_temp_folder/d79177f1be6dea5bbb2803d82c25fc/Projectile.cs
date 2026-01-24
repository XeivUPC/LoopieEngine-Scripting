using System;

namespace Loopie
{
    class Projectile : Component
    {
        public float speed;
        public Vector3 direction;

        public Projectile()
        {
            Console.WriteLine("Projectile constructor!");
        }

        public void OnUpdate()
        {
            transform.position += direction * Time.deltaTime * speed;
        }
    };
}
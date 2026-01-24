using System;

namespace Loopie
{
    public struct Vector4
    {
        public float x, y, z, w;

        public static Vector4 Zero => new Vector4(0.0f);
        public static Vector4 Right => new Vector4(1.0f, 0.0f, 0.0f, 0.0f);
        public static Vector4 Up => new Vector4(0.0f, 1.0f, 0.0f, 0.0f);
        public static Vector4 Forward => new Vector4(0.0f, 0.0f, 1.0f, 0.0f);
        public double magnitude => Math.Sqrt(x * x + y * y + z * z + w * w);

        public Vector4(float scalar)
        {
            x = scalar;
            y = scalar;
            z = scalar;
            w = scalar;
        }

        public Vector4(float x, float y, float z, float w)
        {
            this.x = x;
            this.y = y;
            this.z = z;
            this.w = w;
        }

        public static Vector4 operator +(Vector4 a, Vector4 b)
        {
            return new Vector4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
        }

        public static Vector4 operator -(Vector4 a, Vector4 b)
        {
            return new Vector4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
        }

        public static Vector4 operator *(Vector4 vector, float scalar)
        {
            return new Vector4(vector.x * scalar, vector.y * scalar, vector.z * scalar, vector.w * scalar);
        }

        public static Vector4 operator /(Vector4 v, float scalar)
        {
            return new Vector4(v.x / scalar, v.y / scalar, v.z / scalar, v.w / scalar);
        }
        
        public static bool operator ==(Vector4 a, Vector4 b)
        {
            return (Math.Sqrt(a.x - b.x + a.y - b.y + a.z - b.z + a.w - b.w) < 1e-10f) ? true : false; ;
        }

        public static bool operator !=(Vector4 a, Vector4 b)
        {
            return !(a == b);
        }

        public override bool Equals(object obj)
        {
            return obj is Vector4 v && this == v;
        }

        public static double Distance(Vector4 a, Vector4 b)
        {
            return (a - b).magnitude;
        }

        public void Normalize()
        {
            float mag = (float)magnitude;
            if(mag > 1E-05f)
                this /= mag;
            else
                this = Zero;
        }

        public static float Dot(Vector4 a, Vector4 b)
        {
            return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
        }

        public Vector4 normalized
        {
            get
            {
                float mag = (float)magnitude;
                return mag > 1E-05f? this / mag : Zero;
            }
        }
    }
}
namespace Loopie
{
    public class Time
    {
        public static float deltaTime
        {
            get { return GetDeltaTime(); }
        }
        private float GetDeltaTime()
        {
            return InternalCalls.Time_GetDeltaTime();
        }
    }
}
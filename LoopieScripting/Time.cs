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

        public static float fixedDeltaTime
        {
            get { return GetFixedDeltaTime(); }
        }
        private float GetFixedDeltaTime()
        {
            return InternalCalls.Time_GetFixedDeltaTime();
        }

        public static float timeScale
        {
            get { return GetTimeScale(); }
            set { SetTimeScale(value); }
        }
        private float GetTimeScale()
        {
            return InternalCalls.Time_GetTimeScale();
        }
        private void SetTimeScale(float scale)
        {
            InternalCalls.Time_SetTimeScale(scale);
        }
    }
}
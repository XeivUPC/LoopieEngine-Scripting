namespace Loopie {
	class Time
	{
	public:

		Time();
		~Time() = default;

		void CalculateFrame();

		static void SetFixedDeltaTimeMs(float value);
		static void SetFixedDeltaTime(float value);
		static void SetTimeScale(float value);

		static float DeltaTime() { return m_deltaTimeMs / 1000 * m_timeScale; }
		static float DeltaTimeMs() { return m_deltaTimeMs * m_timeScale; }
		static float FixedDeltaTime() { return m_fixedDeltaTimeMs / 1000 * m_timeScale; }
		static float FixedDeltaTimeMs() { return m_fixedDeltaTimeMs * m_timeScale; }

		static float UnscaledDeltaTime() { return m_deltaTimeMs / 1000; }
		static float UnscaledDeltaTimeMs() { return m_deltaTimeMs; }
		static float UnscaledFixedDeltaTime() { return m_fixedDeltaTimeMs / 1000; }
		static float UnscaledFixedDeltaTimeMs() { return m_fixedDeltaTimeMs; }

		static float RunTime() { return m_runTimeMs / 1000; }
		static float RunTimeMs() { return m_runTimeMs; }
		
		static float ExecutionTime() { return m_executionTimeMs / 1000; }
		static float ExecutionTimeMs() { return m_executionTimeMs; }

		static int FrameCount() { return m_frameCount; }

		static float TimeScale() { return m_timeScale; }

	private:
		float m_lastFrameTime = 0;

		static int m_frameCount;
		//Physic
		static float m_fixedDeltaTimeMs;

		// Game Clock
		static float m_timeScale;
		static float m_deltaTimeMs;
		static float m_runTimeMs;

		//Real Time Clock
		static float m_executionTimeMs;
	};
}
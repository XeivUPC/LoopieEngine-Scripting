#include "Time.h"
#include <SDL3/SDL_timer.h>
namespace Loopie
{
	int Time::m_frameCount = 0;
	float Time::m_fixedDeltaTimeMs = 0;
	float Time::m_timeScale = 0;
	float Time::m_deltaTimeMs = 0;
	float Time::m_runTimeMs = 0;
	float Time::m_executionTimeMs = 0;

	Time::Time()
	{
	}

	void Time::CalculateFrame()
	{
		Uint64 now = SDL_GetPerformanceCounter();
		if (m_lastFrameTime != 0)
		{
			m_deltaTimeMs = (float)((now - m_lastFrameTime) * 1000.0 / (double)SDL_GetPerformanceFrequency());
		}
		else
		{
			m_deltaTimeMs = 0.0f;
		}
		m_lastFrameTime = now;

		m_runTimeMs += DeltaTimeMs();
		m_executionTimeMs += UnscaledDeltaTimeMs();

		m_frameCount++;
	}

	void Time::SetFixedDeltaTimeMs(float value)
	{
		m_fixedDeltaTimeMs = value;
	}

	void Time::SetFixedDeltaTime(float value)
	{
		m_fixedDeltaTimeMs = value * 1000;
	}

	void Time::SetTimeScale(float value)
	{
		m_timeScale = value;
	}
}
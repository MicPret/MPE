#ifndef MPE_TIME_HPP
#define MPE_TIME_HPP

namespace mpe::Time
{
	float GetTime();
	float GetUnscaledTime();
	float GetTimeScale();
	void SetTimeScale(float time_multiplier);
}

#endif
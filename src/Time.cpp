#include <MPE/Core/Time.hpp>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <cassert>

namespace mpe::Time
{
	static float timescale = 1.0f;

	float GetTime()
	{
		return GetUnscaledTime() * timescale;
	}

	float GetUnscaledTime()
	{
		assert(glfwInit() == GLFW_TRUE);
		return static_cast<float>(glfwGetTime());
	}

	float GetTimeScale()
	{
		return 0.0f;
	}

	void SetTimeScale(float time_multiplier)
	{
		timescale = time_multiplier;
	}
}
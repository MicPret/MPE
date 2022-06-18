#include <MPE/Core/Input.hpp>
#include <MPE/Core/Window.hpp>

#include <GLFW/glfw3.h>

namespace mpe::Input
{
	bool IsKeyPressed(char key)
	{
		Window* w = Window::GetActive();
		int result = glfwGetKey(w->GetNativeWindow(), static_cast<int>(key));
		return result == GLFW_PRESS;
	}
}
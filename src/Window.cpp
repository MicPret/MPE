#include <MPE/Core/Window.hpp>
#include <MPE/Core/Logger.hpp>
#include <MPE/Core/Utils.hpp>

#include <glad/gl.h>
#include <glfw/glfw3.h>

namespace mpe
{
	int Window::initialized_windows = 0;

	Window::Window(const std::string& title, int width, int height) :
		win(nullptr),
		w(-1),
		h(-1)
	{
		Logger& logger = Global<Logger>();
		std::string message;

		//Test size
		if ((width < 0) || (height < 0))
		{
			message = "Invalid Window size: width and height must be greater than zero!";
			logger.Log(message, LogSeverity::Error, LogOrigin::MPE);
			return;
		}

		//Test window title
		if (title.empty())
		{
			message = "Window must have a title!";
			logger.Log(message, LogSeverity::Error, LogOrigin::MPE);
			return;
		}

		//Initialize GLFW if needed
		if (initialized_windows == 0)
		{
			if (!InitializeGLFW())
			{
				message = "GLFW failed during initialization!";
				logger.Log(message, LogSeverity::Error, LogOrigin::MPE);
				return;
			}
		}

		//Instantiate the window
		win = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);

		//Test window creation
		if (win == nullptr)
		{
			message = "GLFW failed to create a window!";
			logger.Log(message, LogSeverity::Error, LogOrigin::MPE);
			glfwTerminate();
			return;
		}

		//Let graphics API work on this window
		SetAsGraphicsContext();

		//Load OpenGL if needed
		if (initialized_windows == 0)
		{
			int version = gladLoadGL((GLADloadfunc)glfwGetProcAddress);
			if (version == 0)
			{
				message = "Failed to load OpenGL functions!";
				logger.Log(message, LogSeverity::Error, LogOrigin::MPE);
				Destroy();
				glfwTerminate();
				return;
			}
			else
			{
				message = "Loaded OpenGL functions [version: " + std::to_string(version) + ']';
				logger.Log(message, LogSeverity::Info, LogOrigin::MPE);

				//Debug Output
#ifndef NDEBUG
				auto glDebugOutput = [](
					GLenum source,
					GLenum type,
					unsigned int id,
					GLenum severity,
					GLsizei length,
					const char* message,
					const void* userParam)
				{
					LogSeverity sev = LogSeverity::Warning;
					switch (severity)
					{
					case GL_DEBUG_SEVERITY_NOTIFICATION:
						sev = LogSeverity::Info;
						break;
					case GL_DEBUG_SEVERITY_HIGH:
						sev = LogSeverity::Error;
						break;
					}
					Global<Logger>().Log(std::string(message), sev, LogOrigin::RenderingApi);
				};

				glEnable(GL_DEBUG_OUTPUT);
				glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
				glDebugMessageCallback(glDebugOutput, nullptr);
				glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);
#endif
			}
		}

		w = width;
		h = height;
		initialized_windows++;
	}

	Window::~Window()
	{
		Destroy();
		if (initialized_windows == 0)
			glfwTerminate();
	}

	void Window::SetAsGraphicsContext()
	{
		glfwMakeContextCurrent(win);
	}

	bool Window::IsValid() const
	{
		return win != nullptr;
	}

	void Window::Destroy()
	{
		if (IsValid())
		{
			glfwDestroyWindow(win);
			win = nullptr;
			initialized_windows--;
		}
	}

	int Window::Width() const
	{
		return w;
	}

	int Window::Height() const
	{
		return h;
	}

	void Window::SetClearColor(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
	}

	void Window::BeginFrame()
	{
		//TODO other bits
		glClear(GL_COLOR_BUFFER_BIT);
		//TODO imgui stuff???
	}

	void Window::EndFrame()
	{
		//TODO imgui stuff???
		glfwPollEvents();
		glfwSwapBuffers(win);
	}

	bool Window::ShouldClose() const
	{
		return glfwWindowShouldClose(win);
	}

	GLFWwindow* Window::GetNativeWindow()
	{
		return win;
	}

	const GLFWwindow* Window::GetNativeWindow() const
	{
		return win;
	}

	bool Window::InitializeGLFW()
	{
		if (glfwInit() == GLFW_FALSE)
			return false;

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifndef NDEBUG
		glfwWindowHint(GLFW_CONTEXT_DEBUG, GLFW_TRUE);
#endif

		return true;
	}
}
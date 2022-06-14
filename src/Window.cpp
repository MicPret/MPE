#include <MPE/Core/Window.hpp>
#include <MPE/Core/Log.hpp>
#include <MPE/Core/Utils.hpp>
#include <MPE/Graphics/Renderer.hpp>
#include <MPE/Graphics/Camera.hpp>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace mpe
{
	int Window::initialized_windows = 0;
	static bool InitializeGLFW();

	Window::Window(const std::string& title, int width, int height) :
		win(nullptr),
		w(-1),
		h(-1)
	{
		//Test size
		if ((width < 0) || (height < 0))
		{
			MPE_ERROR("Invalid Window size: width and height must be greater than zero!");
			return;
		}

		//Test window title
		if (title.empty())
		{
			MPE_ERROR("Window must have a title!");
			return;
		}

		//Initialize GLFW if needed
		if (initialized_windows == 0)
		{
			if (!InitializeGLFW())
			{
				MPE_ERROR("GLFW failed during initialization!");
				return;
			}
		}

		//Instantiate the window
		win = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);

		//Test window creation
		if (win == nullptr)
		{
			MPE_ERROR("GLFW failed to create a window!");
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
				MPE_ERROR("Failed to load OpenGL functions!");
				Destroy();
				glfwTerminate();
				return;
			}

			MPE_INFO("Loaded OpenGL functions [version: " + std::to_string(version) + ']');
			
			MPE_INFO("OpenGL version: " + std::string((const char*)glGetString(GL_VERSION)));

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
				std::string msg = "OpenGL code: " + std::to_string(id) + '\n' + message;
				switch (severity)
				{
				case GL_DEBUG_SEVERITY_NOTIFICATION:
					MPE_INFO(msg);
					break;
				case GL_DEBUG_SEVERITY_HIGH:
					MPE_ERROR(msg);
					break;
				default:
					MPE_WARNING(msg);
					break;
				}
			};

			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(glDebugOutput, nullptr);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);
#endif			
		}
		SetClearColor(0.4f, 0.67f, 0.9f);
		glfwSetWindowUserPointer(win, this);
		glfwSetFramebufferSizeCallback(win, [](GLFWwindow* win_, int w_, int h_)
			{
				Window* window = static_cast<Window*>(glfwGetWindowUserPointer(win_));

				float new_aspect = (float)w_ / (float)h_;

				Camera* cam = Renderer::GetActiveCamera();
				if (cam != nullptr)
				{
					Transform t = cam->GetTransform();
					cam = new (cam) Camera(cam->GetFov(), new_aspect, cam->GetNearPlane(), cam->GetFarPlane());
					cam->SetTransform(t);
				}

				glViewport(0, 0, w_, h_);
			});

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

	void Window::Draw()
	{
		glfwSwapBuffers(win);
		glfwPollEvents();
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

	void Window::SetClearColor(float r, float g, float b)
	{
		glClearColor(r, g, b, 1.0f);
	}

	Window* Window::GetActive()
	{
		return static_cast<Window*>(glfwGetWindowUserPointer(glfwGetCurrentContext()));
	}

	bool InitializeGLFW()
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
#include <iostream>

#include <glad/gl.h>
#include <glfw/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <MPE/Core/Logger.hpp>

void glDebugOutput(
	GLenum source,
	GLenum type,
	unsigned int id,
	GLenum severity,
	GLsizei length,
	const char* message,
	const void* userParam)
{
	using namespace mpe;
	Logger& logger = Logger::GetInstance();
	LogSeverity sev;
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		sev = LogSeverity::Info;
		break;
	case GL_DEBUG_SEVERITY_HIGH:
		sev = LogSeverity::Error;
		break;
	default:
		sev = LogSeverity::Warning;
		break;
	}
	logger.Log(message, sev, LogOrigin::RenderingApi);
}

int main()
{
	//Initialize GLFW
	if (!glfwInit())
		return -1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifndef NDEBUG
	glfwWindowHint(GLFW_CONTEXT_DEBUG, GLFW_TRUE);
#endif
	//Create window
	GLFWwindow* window = glfwCreateWindow(800, 600, "Title", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	//Load OpenGL
	int version = gladLoadGL((GLADloadfunc)glfwGetProcAddress);
	if (!version)
	{
		glfwTerminate();
		return -2;
	}
#ifndef NDEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(glDebugOutput, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);
#endif

	//Initialize imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 460");

	bool open = true;
	mpe::Logger& logger = mpe::Logger::GetInstance();

	glClearColor(0.2f, 0.4f, 0.4f, 1.0f);
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		
		ImGui_ImplGlfw_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		if (open)
		{
			if (ImGui::Begin("Test", &open))
			{
				//draw stuff
			}
			ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		logger.Flush();
		glfwPollEvents();
		glfwSwapBuffers(window);
	}


	glfwTerminate();
	return 0;
}
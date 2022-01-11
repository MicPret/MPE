#include <iostream>

#include <glad/gl.h>
#include <glfw/glfw3.h>

int main()
{
	if (!glfwInit())
		return -1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "Title", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	int version = gladLoadGL((GLADloadfunc)glfwGetProcAddress);
	if (!version)
	{
		glfwTerminate();
		return -2;
	}
	glClearColor(0.2f, 0.4f, 0.4f, 1.0f);
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}
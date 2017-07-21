#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>

void window_resize(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void handle_input(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main(int argc, char *argv[])
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(
	                         800, 600,
	                         "Mandelbrot",
	                         NULL, NULL
	                     );

	if (window == nullptr) {
		std::cout << "Failed to create a GLFW window" << std::endl;
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return 1;
	}

	glViewport(0, 0, 800, 600);

	glfwSetFramebufferSizeCallback(window, window_resize);

	while (!glfwWindowShouldClose(window))
	{
		handle_input(window);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

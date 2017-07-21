#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <cmath>
#include <iostream>
#include <memory>

static float r, g, b, a;

void window_resize(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void handle_input(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (!(action == GLFW_PRESS || action == GLFW_REPEAT))
		return;

	switch (key) {
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, true);
		break;
	case GLFW_KEY_LEFT:
		r = fmod(r + 0.1f, 1.1f);
		break;
	case GLFW_KEY_DOWN:
		g = fmod(g + 0.1f, 1.1f);
		break;
	case GLFW_KEY_RIGHT:
		b = fmod(b + 0.1f, 1.1f);
		break;
	case GLFW_KEY_UP:
		a = fmod(a + 0.1f, 1.1f);
	default:
		std::cerr << "Don't know that key!" << std::endl;
	}
	std::cout << "r: " << r << " g: " << g << " b: " << b << " a: " << a
	          << std::endl;
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
	glfwSetKeyCallback(window, handle_input);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	std::cout << "Bye!" << std::endl;

	glfwTerminate();
	return 0;
}

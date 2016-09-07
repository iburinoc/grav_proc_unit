#include <iostream>

#include "gpu/gl.hpp"

#include "options.hpp"

void error_callback(int error, const char *desc) {
	std::cout << "Error: " << desc << std::endl;
}

GLFWwindow *init_gl() {
	if(!glfwInit()) {
		// Failed to initialize
		std::cerr << "Failed to initialize OpenGL" << std::endl;
		return NULL;
	}

	glfwSetErrorCallback(error_callback);	

	GLFWwindow *window = glfwCreateWindow(
		options.dimensions.x,
		options.dimensions.y,
		"Gravity Processing Unit",
		NULL,
		NULL);

	if(!window) {
		return NULL;
	}

	glfwMakeContextCurrent(window);

	glewInit();

	return window;
}

void end_gl(GLFWwindow *window) {
	glfwDestroyWindow(window);
	glfwTerminate();
}

